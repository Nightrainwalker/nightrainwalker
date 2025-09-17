import sys
from decimal import Decimal, ROUND_HALF_UP


def format_ratio_half_up(numerator: int, denominator: int) -> str:
    """Return ratio numerator/denominator rounded HALF_UP to 2 decimals as string.

    Examples: 1/3 -> "0.33", 1/2 -> "0.50"
    """
    if denominator == 0:
        return "0.00"
    ratio = (Decimal(numerator) / Decimal(denominator)).quantize(
        Decimal("0.00"), rounding=ROUND_HALF_UP
    )
    return f"{ratio:.2f}"


def main() -> None:
    data = sys.stdin.read().strip().splitlines()
    if not data:
        return

    try:
        n = int(data[0].strip())
    except Exception:
        # If first token not a plain integer on its own line, try tokenizing all input
        tokens = data[0].split()
        if tokens:
            n = int(tokens[0])
            data = [str(n)] + data[1:]
        else:
            return

    counts = {}
    total = 0

    for i in range(1, min(n, len(data) - 0) + 1):
        line = data[i].strip()
        if not line:
            continue
        parts = line.split()
        if len(parts) < 2:
            continue
        product = parts[0].strip()
        try:
            qty = int(parts[1])
        except Exception:
            # Skip malformed line
            continue
        if not product:
            continue
        key = product[0].upper()
        counts[key] = counts.get(key, 0) + qty
        total += qty

    for key in sorted(counts.keys()):
        ratio_str = format_ratio_half_up(counts[key], total)
        print(f"{key}({ratio_str})")


if __name__ == "__main__":
    main()

