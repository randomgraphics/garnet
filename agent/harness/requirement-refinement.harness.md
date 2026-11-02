# Progressive Requirement Refinement

This is the requirements process within the broader
[`ASEH architecture`](ASEH_DESIGN.harness.md).

## Purpose

An initial request is evidence of what the human wants to achieve, not a
complete implementation contract. It can be incomplete, ambiguous, based on a
mistaken assumption, or internally inconsistent. The harness must investigate
before treating proposed means, feature lists, or architecture as fixed
requirements.

The harness may refine its understanding and recommend changing the scope. It
must not silently redefine the user's intended outcome. Keep the original
request, current interpretation, and every material change linked in a
versioned decision record.

## Working model

For each substantial task, maintain a living requirements record with:

- **Outcomes:** what users should be able to do and why it matters.
- **Constraints:** confirmed limits such as platforms, compatibility, budget,
  performance, licensing, and public API stability.
- **Assumptions:** temporary choices made to keep work moving, each with a
  confidence level, evidence, and a review trigger.
- **Unknowns:** questions whose answers could change architecture, scope, or
  validation.
- **Conflicts:** pairs or groups of requirements that cannot all be satisfied
  under the current interpretation.
- **Acceptance evidence:** observable behavior, benchmarks, builds, tests,
  samples, or device checks that demonstrate progress toward the outcomes.

Do not turn an assumption into a confirmed constraint merely because the code
has been written around it. Keep requirements traceable to the user's request,
repository evidence, experiments, and human decisions.

## Refinement loop

1. **Interpret.** Restate the desired outcome, separate it from the suggested
   implementation, and list likely omissions and contradictions.
2. **Investigate.** Read relevant APIs, implementations, build configuration,
   tests, platform support, and prior task records. Prefer evidence gathered
   from this repository over generic expectations.
3. **Map uncertainty.** Estimate which unknowns affect user value, compatibility,
   architecture, cost, schedule, or safety. Identify which can be answered by
   code inspection or a bounded experiment.
4. **Explore.** Run narrow prototypes, benchmarks, or compatibility checks when
   they can distinguish between plausible designs. Keep experiments isolated
   and record what result would change the decision.
5. **Recommend.** Present a small set of viable options with evidence,
   consequences, and a recommended default. Choose reversible, low-impact
   defaults autonomously and mark them provisional.
6. **Ask selectively.** Ask the human controller only when a decision changes
   product direction, public compatibility, supported platforms, major cost,
   irreversible state, or a value judgment that repository evidence cannot
   resolve. Bundle related questions into one concise decision request.
7. **Replan.** Incorporate new evidence or human direction into a new
   requirements version, update the work graph and acceptance checks, and
   preserve the prior version for auditability.
8. **Validate.** Check the result against the latest agreed outcomes and
   constraints, including decisions made after implementation began. Reopen a
   requirement when new evidence invalidates its premise.

When a decision is pending, continue independent research, prototypes, and
reversible work. Do not cross the decision boundary by making a high-impact
choice on the user's behalf. If the human is unavailable, the harness may
continue with a clearly marked provisional choice only when the choice is
reversible and does not foreclose other options.

## Interaction policy

For each unresolved high-impact item, provide the human with:

- the exact decision to make;
- repository or experiment evidence;
- two or three feasible options and their consequences;
- a recommendation and its confidence;
- work that can proceed independently while the decision is pending.

Do not repeatedly ask for details that can be inferred safely or tested cheaply.
Do not hide open questions in a long plan. Keep the requirements record current
and make the final verification report distinguish confirmed requirements,
provisional assumptions, unsupported platforms, and unverified claims.
