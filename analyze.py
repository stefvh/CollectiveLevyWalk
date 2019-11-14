# Import necessary libraries
import numpy as np 
import powerlaw 
import argparse, os

class Analyze():
    def __init__(self):
        # Directories
        self.dir = "/groups/wall2-ilabt-iminds-be/jnauta/exp/collective_levy/CollectiveLevyWalk/"

    def heavy_tailedness(self, controller, N, s, L=170):
        """ Fit a powerlaw over the data to conclude if the
            distribution is heavy-tailed or not 
        """
        # Initialize parameters
        xmin = 0.17
        xmax = 60*0.17 
        # xmin_candidates = [xmin*i for i in range(1,10+1)]
        xmin_candidates = (xmin, 10*xmin)
        rdir = self.dir+"results/heavy_tailedness_%i/"%(L)
        sdir = self.dir+"output/heavy_tailedness_%i/"%(L)
        if not os.path.exists(sdir):
            os.makedirs(sdir)
        # Load data
        dirname = rdir+"%s/%iN/%i/"%(controller,N,s)
        step_lengths = np.loadtxt(dirname+"heavy_tailedness_step_lengths.txt")
        # Round all data to 2-decimal precision
        step_lengths = np.around(step_lengths, decimals=2)
        # Fit a powerlaw distribution with the given xmin candidates
        fit = powerlaw.Fit(step_lengths, xmin=xmin_candidates, xmax=xmax, verbose=False)
        R, p = fit.distribution_compare('power_law', 'exponential')

        # Store the data
        if type(xmin_candidates) == tuple:
            name = "xmin_range"
        if type(xmin_candidates) == float:
            name = "xmin"
        if type(xmin_candidates) == list:
            name = "xmin_list"
        np.savetxt(sdir+"results%i_%s_%iN_%i_%s.txt"%(L,controller,N,s,name), [R, p, fit.alpha, fit.xmin])

    # def search_efficiency(self, controllers, env):
    #     """ Compute the search efficiency for each controller """        
    #     dx = 0.17
    #     min_dticks = 1

    #     rdir = "results/search_efficiency/"
    #     sdir = "output/"

    #     for c in controllers:
    #         eta = np.zeros((self.N_SIZES,self.N_SEEDS))     # Search efficiency
    #         eps = np.zeros((self.N_SIZES,self.N_SEEDS))     # Unique search efficiency
    #         nu = np.zeros((self.N_SIZES,self.N_SEEDS))      # Patch search efficiency
    #         k = 0
    #         for N in self.swarm_size:
    #             for s in self.seeds:
    #                 print("Computing search efficiency... %s,%i,%i"%(c,N,s))
    #                 dirname = rdir+"%s/%s/%iN/%i/"%(env,c,N,s)
    #                 try:
    #                     state_counters = np.loadtxt(dirname+"search_efficiency_state_counters.txt")                    
    #                     target_findings = np.loadtxt(dirname+"search_efficiency_target_findings.txt")
    #                     # Compute the different search efficiencies
    #                     distance = dx * np.sum(state_counters[:,1]) # Total distrance traversed by the swarm
    #                     n = 1
    #                     prev_robotid = -1 
    #                     prev_targetid = -1
    #                     for i in range(1,target_findings.shape[0]):
    #                         robotid = target_findings[i,1]
    #                         targetid = target_findings[i,2]
    #                         dticks = target_findings[i,0]-target_findings[i-1,0]
    #                         if robotid != prev_robotid:
    #                             n += 1
    #                             prev_robotid = robotid
    #                             prev_targetid = targetid 
    #                         elif targetid == prev_targetid and dticks > min_dticks:
    #                             n += 1
    #                         else:
    #                             pass                        
    #                     u = len(np.unique(target_findings[:,2]))
    #                 except OSError:
    #                     n = 0
    #                     u = 0 
    #                 eta[k,s-1] = n / distance 
    #                 eps[k,s-1] = u / distance 

    #                 # Compute patch search efficiency if environment was patchy
    #                 if env == "patchy":
    #                     pass 
    #             k += 1

    #         # Store means and standard deviation
    #         mean_eta = np.mean(eta, axis=1)
    #         std_eta = np.std(eta, axis=1)
    #         np.save(sdir+"mean_eta_%s"%(c), mean_eta)
    #         np.save(sdir+"std_eta_%s"%(c), std_eta)
    #         mean_eps = np.mean(eps, axis=1)
    #         std_eps = np.std(eps, axis=1)
    #         np.save(sdir+"mean_eps_%s"%(c), mean_eps)
    #         np.save(sdir+"std_eps_%s"%(c), std_eps)

    #         if env == "patchy":
    #             pass 

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--N', dest='swarm_size', type=int)
    parser.add_argument('--s', dest='seed', type=int)
    parser.add_argument('--c', dest='controller', type=str)
    parser.add_argument('--L', dest='L', type=int)
    args = parser.parse_args()

    A = Analyze()
    A.heavy_tailedness(args.controller, args.swarm_size, args.seed, args.L)
    print("Analysis finished for %s, %iN, seed %i"%(args.controller,args.swarm_size,args.seed))

                            





        



