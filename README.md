#LatticeGenerator

Generates triangular lattice for given nodes NX in X direction and NY nodes in the
 Y direction. Bond connection and dihedral connections are generated as well. Each 
node is also given a particleID. 

/*      Particle Typeid         */
/*      Normal nodes - particle Id 0 */
/*      Clamped left - particle Id  1   */
/*      Right end free to slide along X - - particle Id  3 */
/*      Backbone of ribbon - particle Id 4 */

#Output file is generated : ../Sim_dump_ribbon/lattice_L%d_W%d.dat

#Usage: ./LATINITIAL NX NY

#Once lattice_L%d_W%d.dat is generated use python code to generate GSD file.

# python lat.py L-SIZE OF SIMULATION BOX ../Sim_dump_ribbon/lattice_L%d_W%d.dat ../Sim_dump_ribbon/init_strip__L%d_W%d.gsd






