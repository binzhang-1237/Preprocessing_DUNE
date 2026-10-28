# DUNE preprocessing environment

## Versions

| Component | Version |
| --- | --- |
| Python | 3.9.18 |
| NumPy | 1.26.4 |
| h5py | 3.6.0 |
| PyTables | 3.7.0 |
| numexpr | 2.8.1 |
| Numba | 0.58.1 |
| llvmlite | 0.41.1 |
| joblib | 1.3.2 |
| sparse | 0.14.0 |
| SciPy | 1.11.4 |
| matplotlib | 3.5.1 |
| tqdm | 4.66.1 |
| seaborn | 0.13.2 |
| cmasher | 1.8.0 |
| pandas | 2.2.2 |
| PyTorch | 2.0.1, CPU |
| MinkowskiEngine | 0.5.4, CPU |
| JupyterLab | 4.0.11 |
| ipykernel | 6.29.0 |
| pip | 22.0.2 |
| setuptools | 59.6.0 |
| wheel | 0.37.1 |
| cxx-compiler | 1.7.0 |
| OpenBLAS | 0.3.23 |
| ninja | 1.11.1 |

## Create

```bash
TMPDIR=/mnt/ironwolf_14t2/users/binzhang/local/tmp PYTHONNOUSERSITE=1 conda env create -f /home/binzhang/disk_binzhang/preprocessing_multi_production/Preprocessing_Alejandro/environment_preprocessing.yml
```

## Activate

Run after installation finishes successfully:

```bash
conda activate /mnt/ironwolf_14t2/users/binzhang/local/dune-preprocessing
```

## Preprocess

```bash
cd /home/binzhang/disk_binzhang/preprocessing_multi_production/Preprocessing_Alejandro
./run_preprocess2.sh
```

## Notebook

```bash
cd /home/binzhang/disk_binzhang/preprocessing_multi_production/Preprocessing_Alejandro
jupyter lab CreateFullySparseDataset.ipynb
```
