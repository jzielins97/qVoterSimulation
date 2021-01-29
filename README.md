# Q-voter model

## Brief introduction
In this project a code has been written to simulate a q-voter model on a multilayer network (two layers). Each layer is a full graph. Each individual (node) in the graph can be in one of two states (in this implementation true/false is used).\
A q parameter of the model defines the size of the lobby that is randomly selected for each individual out of the other individuals on the layer. With probability of 1-p the lobby can influence the individual if the lobby is homogeneous. In this case, the voter takes the same state as the lobby. With probability p, the voter is not affected by the lobby but still can change his views with 50% chance.\
The model uses LOCAL&AND dynamics, meaning that an individual can be affected by a lobby on one layer, but not on the other. The voter then changes his final view only if it is the same on both layers.

For better explanation please read the paper: https://arxiv.org/abs/1503.01400

## How to use:
### C++
The program simulates the model on N voters with given q parameter. It does R simulation for each noise p (which is 0-1 with a step of 0.02). It creates a file with pairs of p and average magnetization <m> (averaged over R simulations). A return file is saved in the output directory as q_N_R_pmin-pmax_dp.txt.

Compile the program by running: g++ project.cpp -o project

To run: ./projekt <q> <N> <R> optional{<dp> <p_max> <p_min>}\
q - size of the lobby\
N - number of voters\
R - number of simulations to average\
dp - a step for the noise\
p_max - higher limit of the noise range\
p_min - lower limit of the noise range\
NOTE: p_max is given before p_min, because more often the user will want to limit only from the top.

### Python (under development)
The program is written in the jupyter-lab. It can be run directly inside the notebook. Simply run each cell by Shift+Enter.\
In the end the graph is drawn of <m> vs p
