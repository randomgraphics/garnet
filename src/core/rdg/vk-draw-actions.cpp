#include "vk-draw-actions.h"
#include "vk-commandbuffer.h"
#include "vk-renderpass.h"
#include "submission.h"

namespace GN::rdg {

static GN::Logger * sLogger = getLogger("GN.rdg");

class ClearRenderTargetVulkan : public ClearRenderTarget {
public:
    ClearRenderTargetVulkan(ArtifactDatabase & db, const StrA & name)
        : ClearRenderTarget(db, TYPE, name) {}

    std::pair<ExecutionResult, ExecutionContext *> prepare(Submission & submission, Arguments & arguments) override {
        auto submissionImpl = static_cast<SubmissionImpl *>(&submission);
        if (!submissionImpl) GN_UNLIKELY {
            GN_ERROR(sLogger)("ClearRenderTargetVulkan::prepare: submission is not SubmissionImpl");
            return std::make_pair(FAILED, nullptr);
        }

        auto a = arguments.castTo<ClearRenderTarget::A>();
        if (!a) GN_UNLIKELY {
            GN_ERROR(sLogger)("ClearRenderTargetVulkan::prepare: arguments is not ClearRenderTarget::A");
            return std::make_pair(FAILED, nullptr);
        }

        auto ctx = std::make_unique<DrawActionContextVulkan>();

        // prepare command buffer.
        auto cbm = submissionImpl->getExecutionContext<CommandBufferManagerVulkan>();
        if (!cbm) GN_UNLIKELY {
            cbm = AutoRef<CommandBufferManagerVulkan>::make(CommandBufferManagerVulkan::ConstructParameters {.context = submission.context()});
            submissionImpl->setExecutionContext(cbm);
        }
        ctx->commandBufferId = cbm->prepare(CommandBufferManagerVulkan::GRAPHICS);
        if (!ctx->commandBufferId) GN_UNLIKELY {
            GN_ERROR(sLogger)("ClearRenderTargetVulkan::prepare: failed to prepare command buffer");
            return std::make_pair(FAILED, nullptr);
        }

        // prepare render pass
        auto rpm = submissionImpl->getExecutionContext<RenderPassManagerVulkan>();
        if (!rpm) GN_UNLIKELY {
            rpm = AutoRef<RenderPassManagerVulkan>::make(RenderPassManagerVulkan::ConstructParameters {.context = submission.context()});
            submissionmpl->setExecutionContext(rpm);
        }
        ctx->renderPassId = rpm->prepare(a->renderTarget.get());
        if (!ctx->renderPassId) GN_UNLIKELY {
            GN_ERROR(sLogger)("ClearRenderTargetVulkan::prepare: failed to prepare render pass");
            return std::make_pair(FAILED, nullptr);
        }

        // done
        return std::make_pair(PASSED, ctx.release());
    }

    ExecutionResult execute(Submission & submission, Arguments & arguments, ExecutionContext * context) override {
        bool hasWarning = false;

        auto submissionImpl = static_cast<SubmissionImpl *>(&submission);
        if (!submissionImpl) GN_UNLIKELY {
            GN_ERROR(sLogger)("ClearRenderTargetVulkan::prepare: submission is not SubmissionImpl");
            return std::make_pair(FAILED, nullptr);
        }

        auto a = arguments.castTo<ClearRenderTarget::A>();
        if (!a) GN_UNLIKELY {
            GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: arguments is not ClearRenderTarget::A");
            return FAILED;
        }

        auto ctx = static_cast<DrawActionContextVulkan *>(context);
        if (!ctx) GN_UNLIKELY {
            GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: context is not DrawActionContextVulkan");
            return FAILED;
        }

        // acquire command buffer.
        auto cbm = submissionImpl->getExecutionContext<CommandBufferManagerVulkan>();
        if (!cbm) GN_UNLIKELY {
            GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: CommandBufferManagerVulkan not found");
            return FAILED;
        }
        auto cb = cbm->execute(ctx->commandBufferId);
        if (!cb) GN_UNLIKELY {
            GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: failed to acquire command buffer");
            return FAILED;
        }

        // acquire render pass.
        auto rpm = submissionImpl->getExecutionContext<RenderPassManagerVulkan>();
        if (!rpm) GN_UNLIKELY {
            GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: RenderPassManagerVulkan not found");
            return FAILED;
        }
        auto rp = rpm->execute(ctx->renderPassId);
        if (!rp) GN_UNLIKELY {
            GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: failed to acquire render pass");
            return FAILED;
        }

        // begin render pass, if asked to do so.
        if (rp.beginInfo.has_value()) {
            rp.renderPass.cmdBegin(cb.commandBuffer, rp.beginInfo.value());
        } else {
            // TODO: clear render target in the middle of a render pass.
            GN_ERROR(sLogger)("ClearRenderTargetVulkan::execute: clear render target in the middle of a render pass is not supported");
            (void)a;
            hasWarning = true;
        }

        // forward to next subpass, if asked to do so.
        if (rp.next) rp.renderPass.next();

        // end render pass, if asked to do so.
        if (rp.end) rp.renderPass.end();

        // submit command buffer, if asked to do so.
        if (cbm->submit) cb->commandQueue->kickOff(cb.commandBuffer);

        // done
        return hasWarning ? WARNING : PASSED;
    }
};

AutoRef<ClearRenderTarget> ClearRenderTarget::create(ArtifactDatabase & db, const StrA & name, const CreateParameters &) {
    return AutoRef<ClearRenderTarget>(new ClearRenderTargetVulkan(db, name));
}

} // namespace GN::rdg