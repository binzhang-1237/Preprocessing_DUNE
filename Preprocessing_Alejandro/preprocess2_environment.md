# Isolated environment for preprocess2.py

The environment lives at `/home/binzhang/local/preprocess2`. It is separate from
Conda and from `/home/binzhang/local/root2hdf5`; no shell configuration is changed.
The runner uses the environment's Python directly with `-I`, without activation.

Exact Python, library and HDF5 versions are recorded below.
`requirements_preprocess2.txt` records all installed runtime package versions,
including transitive dependencies.

## Run

```bash
./run_preprocess2.sh
```

Default input: `../dune_pixelmap_generator/output/pixel_maps_test.h5`, dataset
`ArrayOfStructures`.

Default output: `output/preprocessed_pixel_maps_test.h5`.

To choose another input or output:

```bash
./run_preprocess2.sh /path/input.h5 /path/output.h5
```

The output directory is created automatically. Existing output files are not
replaced. `preprocess2.py` stores mother-particle PDG codes as `int32` instead of
`int16`: the input includes nuclear PDG code `1000010020`, which exceeds the
`int16` range. This preserves the PDG values and avoids NumPy 2 overflow errors;
other preprocessing logic is unchanged.

## Recreate

```bash
/usr/bin/python3 -m venv "$HOME/local/preprocess2"
"$HOME/local/preprocess2/bin/python" -m pip install -r requirements_preprocess2.txt
"$HOME/local/preprocess2/bin/python" -m pip check
```

Use the recorded Python version when recreating this environment.

## Verified versions

- Python: `3.10.12` (`/home/binzhang/local/preprocess2/bin/python`)
- Platform: `Linux x86_64`
- numpy: `2.2.6`
- h5py: `3.16.0`
- tables: `3.10.1`
- HDF5 linked by h5py: `2.0.0`
- HDF5 linked by PyTables: `1.14.3`
- pip: `22.0.2`; setuptools: `59.6.0`

## Validation

The default runner completed successfully on the upstream test file:

- 94 input/output events.
- 95,058 event-map pixel records and 85,530 retained prong-map pixel records.
- Every sparse coordinate and raw-charge value matched the source records.
- Padding masks and event/prong tensor shapes matched the source.
- The HDF5 output contains 41 datasets.

PyTables reports `NaturalNameWarning` for existing dataset names containing dots
(e.g. `mc.png_label`). These names are retained for downstream compatibility and
can be read with `h5py` or `getattr` in PyTables.
