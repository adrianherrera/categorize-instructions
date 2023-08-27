# Categorize Instructions

Simple Intel PIN tool to categorize dynamically-executed instructions.

To build:

```bash
export PIN_ROOT=/path/to/pin
make
```

To run:

```bash
export PATH=$PIN_ROOT:$PATH
pin -t obj-intel64/categorize-insts -- /path/to/executable
```

This will produce `inst-categories.csv`.
To change the output file, use the `-o` option (to the PIN tool).
To append to an existing output file, use the `-a` option.
