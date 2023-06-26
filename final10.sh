#!/bin/bash

task=$(qsub task10.pbs)
qsub -W depend=afterokarray:$task sum.pbs
