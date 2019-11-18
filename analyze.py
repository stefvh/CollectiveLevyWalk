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

    def search_efficiency(self, env, controller, N, s, L=170):
        """ Compute the search efficiency for each controller """
        deltat = 0.1        
        dx = 0.17 * deltat 
        min_dticks = 1
        n_patches = 10
        M_i = 100

        rdir = "results/search_efficiency_%i/"%(L)
        sdir = "output/search_efficiency_%i/%s/"%(L,controller)
        if not os.path.exists(sdir):
            os.makedirs(sdir)

        dirname = rdir+"%s/%s/%iN/%i/"%(env,controller,N,s)
        state_counters = np.loadtxt(dirname+"search_efficiency_state_counters.txt") 
        
        distances = dx * state_counters[:,1]    # Distance traversed by each individual
        tot_distance = np.sum(distances)        # Total distance traversed by the swarm
        try:            
            # Compute the different search efficiencies if targets have been detected     
            target_findings = np.loadtxt(dirname+"search_efficiency_target_findings.txt", dtype=int)
            n = np.zeros(N, dtype=int)
            m = np.zeros(n_patches, dtype=int) if env == "patchy" else 0
            unique_n = np.zeros(N, dtype=int)
            prev_robotid = -1 
            prev_targetid = -1
            # Find the number of targets detected
            # using non-destructive foraging
            for i in range(1,target_findings.shape[0]):
                robotid = target_findings[i,1]
                targetid = target_findings[i,2]
                dticks = target_findings[i,0]-target_findings[i-1,0]
                if robotid != prev_robotid:
                    n[robotid] += 1
                    unique_n[robotid] = 1
                    prev_robotid = robotid
                    prev_targetid = targetid 
                    if env == "patchy":
                        patch_id = targetid // M_i
                        m[patch_id] += 1
                elif targetid == prev_targetid and dticks > min_dticks:
                    n[robotid] += 1
                else:
                    pass
            # Find the patch coverage
            # targetids = np.unique(target_findings[:,2])
            # patchids = targetids // M_i
            # _, counts = np.unique(patchids, return_counts=True)
            # m = np.array([m_i/M_i for m_i in counts])      
            # Compute patch search efficiency
            if env == "patchy":
                nbar = np.mean(m)
                m = m[m!=0]
                summand = [nm/(1+abs(nbar-nm)/nm) for nm in m]
                norm = len(m) / n_patches
                nu = norm * np.sum(summand) / tot_distance
            else:
                nu = 0
            # Find the number of unique targets found
            u = len(np.unique(target_findings[:,2]))
            # Compute for each number of targets, how many robots
            # have detected that number (Fourier spectrum?)
            bins = [M_i*i for i in range(n_patches)]
            hist, edges = np.histogram(n, bins=bins)
        
            # Compute the search efficiencies
            # eta = np.sum(n / distances) / N
            eta = np.sum(n) / tot_distance
            eps = u / (M_i*n_patches) if env == "patchy" else u / M_i
            # nu = np.sum(m) / n_patches if env == "patchy" else np.sum(m)

        except OSError:
            eta = 0
            eps = 0
            nu = 0
            hist = np.zeros(n_patches-1)
            hist[0] = N
        
        np.savetxt(sdir+"results%i_%s_%s_%iN_%i.txt"%(L,env,controller,N,s), [eta, eps, nu])
        np.savetxt(sdir+"results_hist%i_%s_%s_%iN_%i.txt"%(L,env,controller,N,s), hist)
    

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--N', dest='swarm_size', type=int)
    parser.add_argument('--s', dest='seed', type=int)
    parser.add_argument('--c', dest='controller', type=str)
    parser.add_argument('--L', dest='L', type=int)
    parser.add_argument('--e', dest='env', type=str, default='sparse')
    args = parser.parse_args()

    A = Analyze()
    A.search_efficiency(args.env, args.controller, args.swarm_size, args.seed, args.L)
    print("Search efficiencies computed for %s, %s, %iN, seed %i"\
        %(args.controller,args.env,args.swarm_size,args.seed))

                            





        



