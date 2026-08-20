import sys
from boot import boot
from trip import trip

def main():
    target_dir = sys.argv[1] if len(sys.argv) > 1 else None
    dist = boot(target_dir)
    if dist:
        trips = trip()
        for d in dist:
            for t in trips:
                d.Pump(t.address.encode("utf-8"), t.payload.encode("utf-8"), t.options.encode("utf-8"))

    print(f"{{\n  \"lang\": \"python\",\n  \"status\": \"ready\",\n  \"engines\": {len(dist)}\n}}")

if __name__ == "__main__":
    main()
