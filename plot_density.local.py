import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from datetime import datetime
import argparse
import os
import json


# asciipb to json convert and save

def asciipb_to_json(path_to_asciipb_lapnig, path_to_asciipb_mfm, path_to_json):
    out = {}

    f1 = open(path_to_asciipb_lapnig, "r")
    txt1 = f1.read()
    txt1 = txt1.split('{')[1].split('}')[0]
    for elem in txt1.split('\n'):
        if elem.__contains__(':'):
            k = elem.split(':')[0].split()[-1]
            v = elem.split(':')[1]
            out[k] = v

    f2 = open(path_to_asciipb_mfm, "r")
    txt2 = f2.read()
    txt2 = txt2.split('{')[1].split('}')[0]
    for elem in txt2.split('\n'):
        if elem.__contains__(':'):
            k = elem.split(':')[0].split()[-1]
            v = elem.split(':')[1]
            out[k] = v

    with open(path_to_json, "w") as fp:
        json.dump(out, fp)


asciipb_to_json('resources/tutorial/lapnig_fixed.asciipb', 'resources/tutorial/mfm_fixed.asciipb',
                'resources/tutorial/hypers_vals.local.json')

# define argument parser
parser = argparse.ArgumentParser()

parser.add_argument("--data", nargs='?', default="resources/tutorial/data.csv")
parser.add_argument("--grid", nargs='?', default="resources/tutorial/grid.csv")
parser.add_argument("--density", nargs='?', default="resources/tutorial/out/density_file.csv")
parser.add_argument("--hypers_vals", nargs='?', default="resources/tutorial/hypers_vals.local.json")
parser.add_argument("--save", nargs='?', default="resources/tutorial/out/figs.local/tutorial")
parser.add_argument('--hierarchy', choices=['NNIG', 'LapNIG'])
parser.add_argument('--mixing', choices=['DP', 'MFM'])
parser.add_argument('--title', choices=['Galaxy', 'Tutorial'])

args = parser.parse_args()

# load data
data = pd.read_csv(args.data, names=["val"])
grid = pd.read_csv(args.grid, names=["val"])
len_grid = grid.shape[0]
dens = pd.read_csv(args.density, names=range(len_grid))
with open(args.hypers_vals) as json_file:
    hypers_vals = json.load(json_file)


# plotting utils
def compute_density_on_grid(dens):
    return np.sum(np.exp(dens), axis=0) / dens.shape[0]


def plot_density(data, grid, dens, savepath, hypers_vals, hierarchy, mixing, title):
    density = compute_density_on_grid(dens)
    fig, ax = plt.subplots(figsize=(10, 6))
    ax.set_title("{}_{}_{}".format(title, hierarchy, mixing), fontsize=14)
    ax.hist(data["val"], density=True, bins=30);
    ax.plot(grid["val"], density);

    if not os.path.exists(savepath):
        os.mkdir(savepath)
    textstr = ''
    if hierarchy == 'LapNIG':
        textstr += '\n'.join(["mean: {}".format(hypers_vals["mean"]),
                              "var: {}".format(hypers_vals["var"]),
                              "shape: {}".format(hypers_vals["shape"]),
                              "scale: {}".format(hypers_vals["scale"]),
                              "mh_mean_var: {}".format(hypers_vals["mh_mean_var"]),
                              "mh_log_scale_var: {}".format(hypers_vals["mh_log_scale_var"]),
                              ])
    if mixing == 'MFM':
        textstr += '\n'
        textstr += '\n'.join(["lambda: {}".format(hypers_vals["lambda"]),
                              "gamma: {}".format(hypers_vals["gamma"])])
    props = dict(boxstyle='round', facecolor='wheat', alpha=0.5)

    txt = ax.text(0.75, 0.95, textstr, transform=ax.transAxes, fontsize=12,
                  verticalalignment='top', bbox=props)
    plt.savefig(args.save + "/{}_{}_{}_{}.local.png".format(title, hierarchy, mixing,
                                                            datetime.now().strftime("%d-%m-%Y_%H:%M:%S")),
                bbox_extra_artists=txt)
    plt.savefig(args.save + "/{}_{}_{}_{}.local.png".format(title, hierarchy, mixing,
                                                            datetime.now().strftime("%d-%m-%Y_%H:%M:%S")))


# plot

plot_density(data, grid, dens, args.save, hypers_vals, args.hierarchy, args.mixing, args.title)
