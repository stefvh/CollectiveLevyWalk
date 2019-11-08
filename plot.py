""" Plotting functions """
# Import necessary libraries
import numpy as np 
import matplotlib.pyplot as plt 
import sys 

import powerlaw 

# Set plotting font for TeX labels
plt.rc('text', usetex=True)
plt.rc('font', family='serif')

class Plotter(object):
    """ Class that holds all plotting functions """
    def __init__(self, MAX_N=1000, DELTA_N=50, N_SEEDS=30):
        # Generate lists
        self.seeds = np.arange(1, N_SEEDS+1, 1, dtype=int)
        self.swarm_size = np.arange(DELTA_N, MAX_N+1, DELTA_N)
        self.controllers = ["clw", "ilw"]

        self.N_SEEDS = N_SEEDS
        self.DELTA_N = DELTA_N
        self.N_SIZES = len(self.swarm_size)

    def plot_heavytailedness(self):
        fig, ax = plt.subplots(1,1, figsize=(8,6))
        figp, axp = plt.subplots(1,1, figsize=(8,6))
        """ Plot LLR and corresponding p-value """
        sdir = "output/"
        # Load the data
        markers = ['o', 'D']
        k = 0
        for c in self.controllers:
            mean_LLR = np.load(sdir+"mean_LLR_%s.npy"%(c))
            std_LLR = np.load(sdir+"std_LLR_%s.npy"%(c))
            ax.errorbar(
                self.swarm_size, mean_LLR, yerr=std_LLR, 
                color='k', marker=markers[k], mfc='white'
            )

            mean_pvalue = np.load(sdir+"mean_pvalue_%s.npy"%(c))
            std_pvalue = np.load(sdir+"std_pvalue_%s.npy"%(c))
            axp.errorbar(
                self.swarm_size, mean_pvalue, yerr=std_pvalue,
                color='k', marker=markers[k], mfc='white'    
            )
            k += 1

        # Labels, limits, etc
        ax.set_xlim([0, max(self.swarm_size)+self.DELTA_N])
        ax.set_ylim(bottom=0)
        ax.set_xlabel(r"$N$", fontsize=12)
        ax.set_ylabel(r"LLR", fontsize=12)

        axp.set_xlim([0, max(self.swarm_size)+self.DELTA_N])
        axp.set_ylim(bottom=0)
        axp.set_xlabel(r"$N$", fontsize=12)
        axp.set_ylabel(r"$p_{LLR}$", fontsize=12)

        return fig 
                    



if __name__ == "__main__":
    pjotr = Plotter(MAX_N = 500, DELTA_N = 50, N_SEEDS=5)

    if "analyze" in sys.argv:
        import analyze
        A = analyze.Analyze(MAX_N=500, DELTA_N=50, N_SEEDS=5)
        A.heavy_tailedness()
    
    fig = pjotr.plot_heavytailedness()

    if "save" in sys.argv:
        pass 
    else:
        plt.show()



    
        
