""" Plotting functions """
# Import necessary libraries
import numpy as np 
import matplotlib.pyplot as plt 
import sys 

import powerlaw 

# Set plotting font for TeX labels
#plt.rc('text', usetex=True)
#plt.rc('font', family='serif')

class Plotter(object):
    """ Class that holds all plotting functions """
    def __init__(self, MAX_N=1000, DELTA_N=50, N_SEEDS=30, xminstr="xmin"):
        # Generate lists
        self.seeds = np.arange(1, N_SEEDS+1, 1, dtype=int)
        self.swarm_size = np.arange(DELTA_N, MAX_N+1, DELTA_N)
        self.powlaw_controllers = ["ilw", "clw"]
        self.searcheff_controllers = ["ilw", "clw", "acrw"]

        self.N_SEEDS = N_SEEDS
        self.DELTA_N = DELTA_N
        self.N_SIZES = len(self.swarm_size)
        
        self.xminstr = xminstr

    def plot_heavytailedness(self, L=170):
        fig, ax = plt.subplots(1,1, figsize=(8,6))
        figp, axp = plt.subplots(1,1, figsize=(8,6))
        figa, axa = plt.subplots(1,1, figsize=(8,6))
        figx, axx = plt.subplots(1,1, figsize=(8,6))
        """ Plot LLR and corresponding p-value """
        sdir = "output/heavy_tailedness/"
        # Load the data
        markers = ['o', 'D']
        colors = ['red', 'blue']
        k = 0
        for c in self.powlaw_controllers:
            data = np.loadtxt(sdir+"heavy_tailedness_results_%s_%i_%s.txt"%(c,L,self.xminstr))
            
            ## Plot seperate data
            # log-likelihood ratio
            mean_LLR = data[:,0]
            std_LLR = data[:,1]
            ax.errorbar(
                self.swarm_size, mean_LLR, yerr=std_LLR, 
                capsize=3, color=colors[k], marker=markers[k], mfc='white', label=c
            )
            # p-value
            mean_pvalue = data[:,2]
            std_pvalue = data[:,3]
            axp.errorbar(
                self.swarm_size, mean_pvalue, yerr=std_pvalue,
                capsize=3, color=colors[k], marker=markers[k], mfc='white', label=c
            )
            # Levy parameter
            mean_alpha = data[:,4]
            std_alpha = data[:,5]
            axa.errorbar(
                self.swarm_size, mean_alpha, yerr=std_alpha,
                capsize=3, color=colors[k], marker=markers[k], mfc='white', label=c
            )
            # xmin
            # mean_xmin = data[:,6]
            # std_xmin = data[:,7]
            # axx.errorbar(
            #     self.swarm_size, mean_xmin, yerr=std_xmin,
            #     capsize=3, color=colors[k], marker=markers[k], mfc='white', label=c
            # )
            # k += 1

        # Labels, limits, etc
        ax.set_xlim([0, max(self.swarm_size)+self.DELTA_N])
        # ax.set_ylim(bottom=0)
        ax.set_xlabel(r"$N$", fontsize=12)
        ax.set_ylabel(r"LLR", fontsize=12)
        ax.ticklabel_format(axis='y', style='sci', scilimits=(0,0))
        ax.legend()

        axp.set_xlim([0, max(self.swarm_size)+self.DELTA_N])
        axp.set_ylim(bottom=0)
        axp.set_xlabel(r"$N$", fontsize=12)
        axp.set_ylabel(r"$p_{LLR}$", fontsize=12)
        axp.legend()

        axa.set_xlim([0, max(self.swarm_size)+self.DELTA_N])
        axa.set_ylim(bottom=1.1)
        axa.set_xlabel(r"$N$", fontsize=12)
        axa.set_ylabel(r"$\alpha$", fontsize=12)
        axa.legend()

        axx.set_xlim([0, max(self.swarm_size)+self.DELTA_N])
        axx.set_ylim(bottom=0)
        axx.set_xlabel(r"$N$", fontsize=12)
        axx.set_ylabel(r"$x_{min}$", fontsize=12)
        axx.legend()



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
    
    def combine_heavytailedness_results(self, L=20):
        sdir = "output/heavy_tailedness_%i/"%(L)
        for c in self.powlaw_controllers:
            rdir = "output/heavy_tailedness_%i/%s/"%(L,c)
            LLR = np.zeros((self.N_SIZES,self.N_SEEDS))
            pvalue = np.zeros(LLR.shape)
            alpha = np.zeros(LLR.shape)
            xmin = np.zeros(LLR.shape)
            n = 0
            for N in self.swarm_size:
                for s in self.seeds:
                    # Get the results
                    llr, p, a, xm = np.loadtxt(rdir+"results%i_%s_%iN_%i_%s.txt"%(L,c,N,s,self.xminstr))
                    LLR[n, s-1] = llr 
                    pvalue[n, s-1] = p 
                    alpha[n, s-1] = a
                    xmin[n,s-1] = xm
                n += 1

            # Compute mean and variance
            mean_LLR = np.mean(LLR, axis=1)
            mean_pvalue = np.mean(pvalue, axis=1)
            mean_alpha = np.mean(alpha, axis=1)
            mean_xmin = np.mean(xmin, axis=1)
            std_LLR = np.std(LLR, axis=1)
            std_pvalue = np.std(pvalue, axis=1)
            std_alpha = np.std(alpha, axis=1)
            std_xmin = np.std(xmin, axis=1)
            data = np.array([
                mean_LLR,std_LLR, mean_pvalue,std_pvalue, mean_alpha,std_alpha, mean_xmin,std_xmin
            ]).T

            # Store mean and variance for plotting
            header = "mean_LLR std_LLR mean_pvalue std_pvalue mean_alpha std_alpha mean_xmin std_xmin"
            np.savetxt(sdir+"heavy_tailedness_results_%s_%i_%s.txt"%(c,L,self.xminstr), data, header=header)

                    



if __name__ == "__main__":
    MAX_N = 1000
    DELTA_N = 50
    N_SEEDS = 30
    L = 20
    name = "xmin_range"
    pjotr = Plotter(MAX_N=MAX_N, DELTA_N=DELTA_N, N_SEEDS=N_SEEDS, xminstr=name)

    if "combine" in sys.argv:
        # First combine the data into seperate arrays 
        pjotr.combine_heavytailedness_results(L=L)

    
    # fig = pjotr.plot_heavytailedness(L=L)
    # fig = pjotr.plot_search_efficiency()

    if "save" in sys.argv:
        pass 
    else:
        plt.show()



    
        
