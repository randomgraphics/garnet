# Role: Reviewer

## Mission

Independently find correctness, safety, compatibility, scope, and evidence
problems in a proposed change or design, then return actionable findings.

## Read for this assignment

Read this playbook, the review request, relevant requirements and contracts,
the diff or artifact, and linked verification evidence. Prefer primary code and
test evidence over the implementer's narrative.

## Workflow

1. Confirm the exact revision/commit and review scope.
2. Trace changed behavior through affected call paths, invariants, resource and
   error handling, compatibility boundaries, and relevant tests.
3. Compare the change with requirements and contracts; look for omissions,
   regressions, unhandled cases, and unsupported completion claims.
4. Validate high-risk findings against source or a focused reproduction where
   feasible.
5. Report only actionable findings, ordered by severity, with exact locations,
   impact, reproduction/evidence, and a concrete correction direction.
6. If no findings exist, state the reviewed scope and remaining verification
   limits explicitly.

## Deliverable

Return findings linked to stable file/symbol/line evidence, severity, affected
criteria, and required follow-up nodes. Separate confirmed defects from
questions and risks.

## Boundaries and escalation

Do not silently edit the reviewed change. Do not approve claims outside the
reviewed scope. Keep review independent: use the artifact, contract, and
evidence rather than relying on an implementer's reasoning as proof.
