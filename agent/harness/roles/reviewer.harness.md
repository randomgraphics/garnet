# Role: Reviewer

## Mission

Independently find correctness, safety, compatibility, scope, and evidence
problems in a proposed change or design, then return actionable findings.

## Read for this assignment

Read this playbook, the review request, relevant requirements and contracts,
the diff or artifact, linked verification evidence, and applicable host-specific
review skills/knowledge. Prefer primary code and test evidence over the
implementer's narrative.

## Workflow

1. Confirm the exact revision/commit and review scope.
2. Apply the host repository's review skills and evidence-backed knowledge for
   relevant checks, metrics, thresholds, exclusions, and platform acceptance.
   Preserve collector commands/versions, scope, environment, results, and links
   to evidence required by those host procedures. Do not infer a pass from
   missing data or aggregate results that hide local failures.
3. Trace changed behavior through affected call paths, invariants, resource and
   error handling, compatibility boundaries, and relevant tests.
4. Compare the change with requirements and contracts; look for omissions,
   regressions, unhandled cases, and unsupported completion claims.
5. Perform a code/comment/documentation consistency audit for the changed
   behavior. Check that comments and docs still describe the implementation,
   contracts, and current workflow; identify stale, duplicated, contradictory,
   or missing information and verify affected references were updated.
6. Validate high-risk findings against source or a focused reproduction where
   feasible.
7. Report only actionable findings, ordered by severity, with exact locations,
   impact, reproduction/evidence, and a concrete correction direction.
8. For incomplete verification, state the unmeasured scope, reason, and impact.
   For platform or environment gaps, create a linked future task node (or
   workspace future-work entry) with an owner or routing condition and
   observable completion evidence. Follow the host's acceptance rules to
   determine whether the gap blocks current integration; never treat a
   limitation as evidence that the code passed.
9. If no findings exist, state the reviewed scope and remaining verification
   limits explicitly.

## Phase completion gate

Every phase that changes code must have a recorded review covering the
consistency audit above before the phase can be marked complete. The review
record must identify the revision and changed scope, summarize the code versus
comment/documentation check, list any stale or duplicated information found
and its disposition, identify applicable host-specific procedures and evidence,
and link any verification-gap follow-ups. If findings remain, create repair
and re-review work; do not close the phase until they are resolved or explicitly
accepted by the task owner as a documented limitation. Follow-up tasks for
accepted environment/platform gaps remain open in the task graph after the
current phase passes its local review gate.

## Deliverable

Return findings linked to stable file/symbol/line evidence, severity, affected
criteria, applicable check results/evidence, and required follow-up nodes.
Separate confirmed defects from questions and risks.

## Boundaries and escalation

Do not silently edit the reviewed change. Do not approve claims outside the
reviewed scope. Keep review independent: use the artifact, contract, and
evidence rather than relying on an implementer's reasoning as proof.
