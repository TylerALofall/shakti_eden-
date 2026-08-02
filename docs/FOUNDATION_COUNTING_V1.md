# Foundation and Counting Contract — Revision 8

## Foundation 0

Foundation 0 contains every ASCII code from 32 through 126 exactly once.

Each stone stores the exact character in `<text>`. Unsafe filenames use the deterministic `ascii_NNN` key while preserving the original character in XML.

The written form is generated from the canonical C 8x8 character atlas. The visual SVG renders the same tile at a larger scale.

Foundation is staged until every required sound is approved.

## Counting zero through ten

Each counting stone contains:

```xml
<numeral>N</numeral>
<quantity>N</quantity>
<duration_ms>N000</duration_ms>
```

The solo and timed-light tablets use the same canonical channel files. The timed-light host must measure elapsed time with a monotonic timer and log the start and end event identities. The static SVG does not pretend to be a live duration.

Zero uses absence of dots. The supplied recording is normalized into `zero.wav` and `0.wav` as mono 16-bit PCM at 16 kHz, with 0.2 seconds of silence before and after the spoken content.

## Confidence and lock eligibility

Confidence is exact validation coverage:

```text
confidence = verified required checks / all required checks
```

The numerator and denominator are always stored and displayed.

A stone is lock-eligible only at 100 percent.

A tablet is lock-eligible only when every stone is 100 percent.

Eden is lock-eligible only when every manifest tablet is ready and 100 percent.

No cryptographic lock is applied in this revision.
