# parallel-graph-centrality
Parallel implementation of graph centrality using MPI and OpenMP.

Finds centrality of a given graph using two metrics:
- minimum total distance (closeness)
- minimum maximum distance (eccentrictiy)

Takes input of the form as the first argument:
```
Number of nodes
Number of edges
Node-name
...
Node-name
Edge-start-index edge-end-index edge-weight
...
```
For example:
```
5
6
Node1
Node2
Node3
Node4
Node5
0 1 3
0 2 4
0 3 9
0 4 1
1 2 3
2 0 3
```

Note that edge weights must be non-negative.

Edit `go.sh` to customise the parallel configuration. Designed to run on a Slurm cluster.
