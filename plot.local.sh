#!/usr/bin/env bash

#python3 plot_density.local.py \
#  --data "resources/tutorial/data.csv" \
#  --grid "resources/tutorial/grid.csv" \
#  --density "resources/tutorial/out/density_file.csv" \
#  --vals "resources/tutorial/lapnig_fixed.local.json" \
#  --save "resources/tutorial/out/figs.local/tutorial" \

python3 plot_density.local.py \
  --data "resources/datasets/galaxy.csv" \
  --grid "resources/datasets/galaxy_grid.csv" \
  --density "resources/tutorial/out/galaxy_density_file.csv" \
  --hypers_vals "resources/tutorial/hypers_vals.local.json" \
  --save "resources/tutorial/out/figs.local/galaxy" \
  --hierarchy 'LapNIG' \
  --mixing 'MFM' \
  --title 'Galaxy'
