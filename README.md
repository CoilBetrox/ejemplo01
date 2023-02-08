#MPI

## Instalacion MPI en Ubuntu

```
sudo apt-get update
sudo apt-get install libopenmpi-dev 

mpiexec --version
```

##compilar

```
mpicxx main.cpp -o ejemplo01
```

```
ejecutar
```
mpiexec -n 4 ./nameProject