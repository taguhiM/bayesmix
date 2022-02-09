#!/usr/bin/env bash

#build/run_mcmc \
#  --algo-params-file resources/tutorial/algo.local.asciipb \
#  --hier-type NNIG --hier-args resources/tutorial/nnig_ngg.asciipb \
#  --mix-type MFM --mix-args resources/tutorial/mfm_fixed.local.asciipb \
#  --coll-name resources/tutorial/out/chains.recordio \
#  --data-file resources/datasets/galaxy.csv \
#  --grid-file resources/datasets/galaxy_grid.csv \
#  --dens-file resources/tutorial/out/density_file.csv \
#  --n-cl-file resources/tutorial/out/numclust.csv \
#  --clus-file resources/tutorial/out/clustering.csv \
  #--best-clus-file resources/tutorial/out/best_clustering.csv

#  --mix-type MFM --mix-args resources/tutorial/mfm_fixed.local.asciipb \
#  --mix-type DP --mix-args resources/tutorial/dp_gamma.asciipb \

for i in {1..2};
do
  build/run_mcmc \
    --algo-params-file resources/tutorial/algo.local.asciipb \
    --hier-type NNIG --hier-args resources/tutorial/nnig_ngg.asciipb \
    --mix-type MFM --mix-args resources/tutorial/mfm_fixed.asciipb \
    --coll-name resources/tutorial/out/chains.recordio \
    --data-file resources/datasets/galaxy.csv \
    --grid-file resources/datasets/galaxy_grid.csv \
    --dens-file resources/tutorial/out/density_file.csv \
    --n-cl-file resources/tutorial/out/numclust.csv \
    --clus-file resources/tutorial/out/clustering.csv \
;done
