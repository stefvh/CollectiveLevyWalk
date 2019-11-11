# Import necessary libraries
import numpy as np 
import powerlaw 

class Analyze():
    def __init__(self, MAX_N=1000, DELTA_N=50, N_SEEDS=30):
        # Generate lists
        self.seeds = np.arange(1, N_SEEDS+1, 1, dtype=int)
        self.swarm_size = np.arange(DELTA_N, MAX_N+1, DELTA_N)
        self.controllers = ["clw", "ilw"]

        self.N_SEEDS = N_SEEDS
        self.N_SIZES = len(self.swarm_size)

    def heavy_tailedness(self):
        """ Fit a powerlaw over the data to conclude if the
            distribution is heavy-tailed or not 
        """
        xmin = 0.17 
        xmax = 60*0.17 
        rdir = "results/heavy_tailedness/"
        sdir = "output/"

        for c in self.controllers:
            LLR = np.zeros((self.N_SIZES, self.N_SEEDS))
            pvalue = np.zeros(LLR.shape)
            i = 0
            for n in self.swarm_size:
                for s in self.seeds:
                    dirname = rdir+"%s/%iN/%i/"%(c,n,s)
                    step_lenghts = np.loadtxt(dirname+"heavy_tailedness_step_lengths.txt")
                    fit = powerlaw.Fit(step_lenghts, xmin=xmin, xmax=xmax, verbose=False)
                    R, p = fit.distribution_compare('truncated_power_law', 'exponential')
                    LLR[i, s-1] = R 
                    pvalue[i, s-1] = p
                    print("Fitting powerlaw... %s,%i,%i,%f"%(c,n,s,p), end='\r')
                i += 1
            # Log-likelihood ratio
            mean_LLR = np.mean(LLR, axis=1)
            std_LLR = np.std(LLR, axis=1)
            np.save(sdir+"mean_LLR_%s"%(c), mean_LLR)
            np.save(sdir+"std_LLR_%s"%(c), std_LLR)
            # p-value
            mean_pvalue = np.mean(pvalue, axis=1)
            std_pvalue = np.std(pvalue, axis=1)
            np.save(sdir+"mean_pvalue_%s"%(c), mean_pvalue)
            np.save(sdir+"std_pvalue_%s"%(c), std_pvalue)

    def search_efficiency(self, controllers, env):
        """ Compute the search efficiency for each controller """        
        dx = 0.17
        min_dticks = 1

        rdir = "results/search_efficiency/"
        sdir = "output/"

        for c in controllers:
            eta = np.zeros((self.N_SIZES,self.N_SEEDS))     # Search efficiency
            eps = np.zeros((self.N_SIZES,self.N_SEEDS))     # Unique search efficiency
            nu = np.zeros((self.N_SIZES,self.N_SEEDS))      # Patch search efficiency
            k = 0
            for N in self.swarm_size:
                for s in self.seeds:
                    print("Computing search efficiency... %s,%i,%i"%(c,N,s))
                    dirname = rdir+"%s/%s/%iN/%i/"%(env,c,N,s)
                    try:
                        state_counters = np.loadtxt(dirname+"search_efficiency_state_counters.txt")                    
                        target_findings = np.loadtxt(dirname+"search_efficiency_target_findings.txt")
                        # Compute the different search efficiencies
                        distance = dx * np.sum(state_counters[:,1]) # Total distrance traversed by the swarm
                        n = 1
                        prev_robotid = -1 
                        prev_targetid = -1
                        for i in range(1,target_findings.shape[0]):
                            robotid = target_findings[i,1]
                            targetid = target_findings[i,2]
                            dticks = target_findings[i,0]-target_findings[i-1,0]
                            if robotid != prev_robotid:
                                n += 1
                                prev_robotid = robotid
                                prev_targetid = targetid 
                            elif targetid == prev_targetid and dticks > min_dticks:
                                n += 1
                            else:
                                pass                        
                        u = len(np.unique(target_findings[:,2]))
                    except OSError:
                        n = 0
                        u = 0 
                    eta[k,s-1] = n / distance 
                    eps[k,s-1] = u / distance 

                    # Compute patch search efficiency if environment was patchy
                    if env == "patchy":
                        pass 
                k += 1

            # Store means and standard deviation
            mean_eta = np.mean(eta, axis=1)
            std_eta = np.std(eta, axis=1)
            np.save(sdir+"mean_eta_%s"%(c), mean_eta)
            np.save(sdir+"std_eta_%s"%(c), std_eta)
            mean_eps = np.mean(eps, axis=1)
            std_eps = np.std(eps, axis=1)
            np.save(sdir+"mean_eps_%s"%(c), mean_eps)
            np.save(sdir+"std_eps_%s"%(c), std_eps)

            if env == "patchy":
                pass 



                            





        



