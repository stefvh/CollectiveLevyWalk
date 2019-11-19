""" Plotting functions """
# Import necessary libraries
import numpy as np 
import matplotlib.pyplot as plt 
import sys, os

import powerlaw 

# Set plotting font for TeX labels
#plt.rc('text', usetex=True)
#plt.rc('font', family='serif')

class Plotter(object):
    """ Class that holds all plotting functions """
    def __init__(self, MIN_N=50, MAX_N=1000, DELTA_N=50, N_SEEDS=30):
        # Generate lists
        self.seeds = np.arange(1, N_SEEDS+1, 1, dtype=int)
        self.swarm_size = np.arange(MIN_N, MAX_N+1, DELTA_N)
        self.powlaw_controllers = ["ilw", "clw"]
        self.searcheff_controllers = ["ilw", "clw"]
        self.target_distributions = ["sparse", "patchy"]

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

    def plot_search_efficiency(self):
        fig, ax = plt.subplots(1,1, figsize=(8,6))
        """ Plot the search efficiency for each of the controllers """
        markers = ['o', 'D', '^']
        sdir = "output/"
        k = 0
        for c in self.searcheff_controllers:
            mean_eta = np.load(sdir+"mean_eta_%s.npy"%(c))
            std_eta = np.load(sdir+"std_eta_%s.npy"%(c))
            ax.errorbar(self.swarm_size, mean_eta, color='k', marker=markers[k], mfc='white')

            k += 1
    
        # Labels, limits, etc
        ax.set_xlabel(r"N")
        ax.set_ylabel(r"$\eta$")
        return fig 
    
    def combine_searchefficiency_results(self, L=170):
        for c in self.searcheff_controllers:
            rdir = "output/search_efficiency_%i_nestedtrue/%s/"%(L,c)
            sdir = "output/search_efficiency_%i_nestedtrue/"%(L)
            for td in self.target_distributions:
                eta = np.zeros((self.N_SIZES,self.N_SEEDS))
                eps = np.zeros((self.N_SIZES,self.N_SEEDS))
                nu = np.zeros((self.N_SIZES,self.N_SEEDS))
                n = 0
                for N in self.swarm_size:
                    for s in self.seeds:
                        # Get the results
                        try:
                            tempeta, tempeps, tempnu = np.loadtxt(rdir+"results%i_%s_%s_%iN_%i.txt"%(L,td,c,N,s))
                        except:
                            tempeta = 0
                            tempeps = 0
                            tempnu = 0
                        eta[n,s-1] = tempeta 
                        eps[n,s-1] = tempeps
                        nu[n,s-1] = tempnu                
                    n += 1
                # Compute mean and variance
                mean_eta = np.mean(eta, axis=1)
                std_eta = np.std(eta, axis=1)
                mean_eps = np.mean(eps, axis=1)
                std_eps = np.std(eps, axis=1)
                mean_nu = np.mean(nu, axis=1)
                std_nu = np.std(nu, axis=1)

                # Store mean and variance for plotting
                data = np.array([
                    mean_eta, std_eta, mean_eps, std_eps, mean_nu, std_nu
                ]).T
                header = "mean_eta std_eta mean_eps std_eps mean_nu std_nu"
                np.savetxt(sdir+"search_efficiency_results_%s_%s_%i.txt"%(c,td,L), data, header=header)

    def combine_searchhistogram_results(self, L=170):
        sdir = "output/search_efficiency_%i/histograms/"%(L)
        if not os.path.exists(sdir):
            os.makedirs(sdir)

        for c in self.searcheff_controllers:
            rdir = "output/search_efficiency_%i/%s/"%(L,c)
            for td in self.target_distributions:
                n = 0
                for N in self.swarm_size:
                    histogram = np.zeros((9, self.N_SEEDS))
                    for s in self.seeds:
                        # Get the results
                        hist = np.loadtxt(rdir+"results_hist%i_%s_%s_%iN_%i.txt"%(L,td,c,N,s)) 
                        histogram[:,s-1] = hist
                    mean_hist = np.mean(histogram, axis=1)
                    std_hist = np.std(histogram, axis=1)
                    HIST = np.array([mean_hist,std_hist]).T
                    # Store the mean and standard deviation for plotting
                    np.savetxt(sdir+"histogram_%s_%s_%i_%i.txt"%(c,td,L,N), HIST)

                        



if __name__ == "__main__":
    MIN_N = 50
    MAX_N = 1000
    DELTA_N = 50
    N_SEEDS=30
    L = 170
    pjotr = Plotter(MIN_N=MIN_N, MAX_N=MAX_N, DELTA_N=DELTA_N, N_SEEDS=N_SEEDS)

    if "combine" in sys.argv:
        # First combine the data into seperate arrays
        pjotr.combine_searchefficiency_results(L=L)
        # pjotr.combine_searchhistogram_results(L=L)
    
    # fig = pjotr.plot_search_efficiency()

    if "save" in sys.argv:
        pass 
    else:
        plt.show()



    
        
