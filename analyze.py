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
        print("C")
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



