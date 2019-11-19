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
    def __init__(self, MIN_N=50, MAX_N=1000, DELTA_N=50, N_SEEDS=30, xminstr="xmin"):
        # Generate lists
        self.seeds = np.arange(1, N_SEEDS+1, 1, dtype=int)
        self.swarm_size = np.arange(MIN_N, MAX_N+1, DELTA_N)
        self.powlaw_controllers = ["ilw", "clw"]
        self.searcheff_controllers = ["acrw"]
        self.target_distributions = ["sparse", "patchy"]

        self.N_SEEDS = N_SEEDS
        self.DELTA_N = DELTA_N
        self.N_SIZES = len(self.swarm_size)
        
        self.xminstr = xminstr

    def plot_heavytailedness(self, L=170):
        fig, ax = plt.subplots(1,1, figsize=(4,3))
        figname = "LLR_%i"%(L)
        figp, axp = plt.subplots(1,1, figsize=(4,3))
        figpname = "pvalue_%i"%(L)
        figa, axa = plt.subplots(1,1, figsize=(4,3))
        figaname = "alpha_%i"%(L)
        figx, axx = plt.subplots(1,1, figsize=(4,3))
        figxname = "xmin_%i"%(L)
        # xaxis = np.pi*(0.5*0.17)**2 * self.swarm_size / L**2
        # maxx = max(np.around(xaxis,decimals=2))
        xaxis = self.swarm_size
        maxx = max(self.swarm_size)+self.DELTA_N
        """ Plot LLR and corresponding p-value """
        sdir = "output/heavy_tailedness_20/"
        # Load the data
        markers = ['o', 'D']
        colors = ['gray', 'black']
        labels = ["ILW", "CLW"]
        k = 0
        for c in self.powlaw_controllers:
            data = np.loadtxt(sdir+"heavy_tailedness_results_%s_%i_%s.txt"%(c,L,self.xminstr))
            
            ## Plot seperate data
            # log-likelihood ratio
            mean_LLR = data[:,0]
            std_LLR = data[:,1]
            ax.errorbar(
                xaxis, mean_LLR, yerr=std_LLR, markersize=3.5,
                capsize=3, color=colors[k], marker=markers[k], mfc='white', label=labels[k]
            )
            # p-value
            mean_pvalue = data[:,2]
            std_pvalue = data[:,3]
            axp.errorbar(
                xaxis, mean_pvalue, yerr=std_pvalue, markersize=3.5,
                capsize=3, color=colors[k], marker=markers[k], mfc='white', label=labels[k]
            )
            # Levy parameter
            mean_alpha = data[:,4]-1
            std_alpha = data[:,5]
            axa.errorbar(
                xaxis, mean_alpha, yerr=std_alpha, markersize=3.5,
                capsize=3, color=colors[k], marker=markers[k], mfc='white', label=labels[k]
            )
            # xmin
            mean_xmin = data[:,6]
            std_xmin = data[:,7]
            axx.errorbar(
                xaxis, mean_xmin, yerr=std_xmin, markersize=3.5,
                capsize=3, color=colors[k], marker=markers[k], mfc='white', label=labels[k]
            )
            k += 1

        # Labels, limits, etc
        # ax.set_xlim([0, maxx])
        # ax.set_ylim(bottom=0)
        ax.set_xlim([0, maxx])
        ax.set_xlabel(r"$N$", fontsize=12)
        ax.set_ylabel(r"LLR", fontsize=12)
        ax.ticklabel_format(axis='y', style='sci', scilimits=(0,0))
        ax.legend(fontsize=12)
        # pvalue
        # set horizontal bar at p=0.05
        p = 0.05
        axp.plot([0,maxx],[p,p], color='k', linestyle='--')
        axp.text(0.1,1/8-0.02,r"$p=0.05$", fontsize=11, transform=axp.transAxes)
        axp.set_xlim([0, maxx])
        axp.set_ylim(bottom=0, top=0.8)
        axp.set_xlabel(r"$N$", fontsize=12)
        axp.set_ylabel(r"$p$", fontsize=12)
        # axp.legend(fontsize=12)
        # alpha
        # set horizontal bar at alpha=1
        axa.plot([0,max(self.swarm_size)], [2,2], color='k', linestyle='--')
        # @TODO: Add Levy/Brownian text to denote the split
        axa.text(0.75,1/3-0.07,r"L\'evy", fontsize=11, transform=axa.transAxes)
        axa.text(0.75,1/3+0.02,r"Brownian", fontsize=11, transform=axa.transAxes)
        axa.set_xlim([0, maxx])
        axa.set_ylim(bottom=0.)
        axa.set_xlabel(r"$N$", fontsize=12)
        axa.set_ylabel(r"$\alpha$", fontsize=12)
        # axa.legend(fontsize=12)

        axx.set_xlim([0, maxx])
        axx.set_ylim(bottom=0)
        axx.set_xlabel(r"$N$", fontsize=12)
        axx.set_ylabel(r"$x_{min}$", fontsize=12)
        # axx.legend(fontsize=12)

        # Create dictionary that contains all the figures:
        figs = {
            figname: fig,
            figpname: figp,
            figaname: figa,
            figxname: figx
        }
        return figs

    def plot_search_efficiency(self, nested="true", L=170):
        """ Plot the search efficiency for each of the controllers """
        # plt.style.use('grayscale')
        colors = ['purple', 'green', 'black']
        labels = ["ACLW", "CLW", "ILW"]
        titles = ["homogeneous", "heterogeneous"]
        markers = ['o', 'D', '^']
        sdir = "output/search_efficiency_%i_nested%s/"%(L, nested)
        i = 0
        figdict = {}
        for td in self.target_distributions:
            figeta, axeta = plt.subplots(1,1, facecolor='white', figsize=(6,4))
            etaname = "eta_%s"%(td)
            figeps, axeps = plt.subplots(1,1, facecolor='white', figsize=(6,4))
            epsname = "eps_%s"%(td)
            fignu, axnu = plt.subplots(1,1, facecolor='white', figsize=(6,4))
            nuname = "nu_%s"%(td)
            k = 0
            for c in self.searcheff_controllers:            
                data = np.loadtxt(sdir+"search_efficiency_results_%s_%s_%i.txt"%(c,td,L))
                # Eta
                mean_eta = data[:,0]
                std_eta = data[:,1]
                axeta.errorbar(
                    self.swarm_size, mean_eta, yerr=std_eta, capsize=3, 
                    marker=markers[k], mfc='white', label=labels[k], color=colors[k]
                )
                # Epsilon
                mean_eps = data[:,2]
                std_eps = data[:,3]
                axeps.errorbar(
                    self.swarm_size, mean_eps, yerr=std_eps, capsize=3,
                    marker=markers[k], mfc='white', label=labels[k], color=colors[k]
                )
                # Nu 
                mean_nu = data[:,4]
                std_nu = data[:,5]
                axnu.errorbar(
                    self.swarm_size, mean_nu, yerr=std_nu, capsize=3,
                    marker=markers[k], mfc='white', label=labels[k], color=colors[k]
                )
                k += 1

        
            # Labels, limits, etc
            axeta.set_title(titles[i], fontsize=18)
            axeta.ticklabel_format(axis='y', style='sci', scilimits=(0,0))
            axeta.set_xlabel(r"$N$", fontsize=17)
            axeta.set_ylabel(r"$\eta$", fontsize=17)
            axeta.tick_params(axis='both', labelsize=14)
            axeta.yaxis.get_offset_text().set_fontsize(14)
            # axeta.text(0.5,0.9,td, fontsize=16, transform=axeta.transAxes)
            if i == 0:
                axeta.legend(fontsize=13)
                axeps.legend(fontsize=13)

            axeps.set_xlabel(r"$N$", fontsize=17)
            axeps.set_ylabel(r"$\epsilon$", fontsize=17)
            # axeps.text(0.5,0.9,td, fontsize=12, transform=axeps.transAxes)
            axeps.set_title(titles[i], fontsize=18)
            axeps.tick_params(axis='both', labelsize=14)
            # axeps.ticklabel_format(axis='y', style='sci', scilimits=(0,0))
            # axeps.yaxis.get_offset_text().set_fontsize(13)

            axnu.set_xlabel(r"$N$", fontsize=17)
            axnu.set_ylabel(r"$\nu$", fontsize=17)
            axnu.tick_params(axis='both', labelsize=14)
            axnu.ticklabel_format(axis='y', style='sci', scilimits=(0,0))
            axnu.yaxis.get_offset_text().set_fontsize(14)

            # Add the figures to the dictionary
            figdict[etaname] = figeta
            figdict[epsname] = figeps
            figdict[nuname] = fignu
            i += 1
        
        return figdict

    def plot_targetfrequency(self, L=170):
        fig, (ax, ax2) = plt.subplots(2,3, sharex=True, figsize=(8,2))
        figname = "hist_aclw"
        plt.subplots_adjust(wspace=0.3, hspace=0.15)
        N = [50, 500, 1000]
        c = "acrw"
        rdir = "output/search_efficiency_%i/"%(L)
        i = 0
        M_i = 100
        n_patches = 10
        x = [M_i*i for i in range(n_patches)][:-1]
        width = np.diff(x)[0]
        for n in N:
            data = np.loadtxt(rdir+"histograms/histogram_%s_patchy_%i_%i.txt"%(c,L,n))
            mean_hist, std_hist = data[:,0], data[:,1]
            indices = np.where((mean_hist-std_hist)<0)
            std_hist_low = std_hist
            std_hist_low[indices] = mean_hist[indices]
            ax[i].bar(
                x, mean_hist, width=width, yerr=np.array([std_hist_low, std_hist]),
                capsize=3, color='gray', align='edge'
            )
            ax2[i].bar(
                x, mean_hist, width=width, yerr=np.array([std_hist_low, std_hist]),
                capsize=3, color='gray', align='edge'
            )
            # Set limits
            ax[i].set_ylim([n/2,n+n/11])
            ax2[i].set_ylim([0,n/50+1])
            if i == 0:
                ax2[i].set_yticks([0,1])
            if i == 2:
                ax[i].set_yticks([600,800,1000])
            # Hide the spines
            ax[i].set_xlim([0,M_i*n_patches])
            ax[i].spines['bottom'].set_visible(False)
            ax[i].tick_params(bottom=False)
            ax2[i].spines['top'].set_visible(False)
            ax2[i].set_xticks([100*i for i in range(11)])
            ax2[i].ticklabel_format(axis='x', style='sci', scilimits=(2,2))
            # Plot the diagonal break lines
            d = .03  # how big to make the diagonal lines in axes coordinates
            # arguments to pass to plot, just so we don't keep repeating them
            kwargs = dict(transform=ax[i].transAxes, color='k', clip_on=False)
            ax[i].plot((-d, +d), (-d, +d), **kwargs)        # top-left diagonal
            ax[i].plot((1 - d, 1 + d), (-d, +d), **kwargs)  # top-right diagonal

            kwargs.update(transform=ax2[i].transAxes)  # switch to the bottom axes
            ax2[i].plot((-d, +d), (1 - d, 1 + d), **kwargs)  # bottom-left diagonal
            ax2[i].plot((1 - d, 1 + d), (1 - d, 1 + d), **kwargs)  # bottom-right diagonal

            # Labels, text, etc
            ax2[i].set_xlabel(r"\#targets", fontsize=14)
            ax[i].text(0.45,0.6, r"$N=%i$"%(n), fontsize=14, transform=ax[i].transAxes)
            i += 1
        fig.text(0.06,0.22, r"\#individuals", rotation='vertical', fontsize=14)
        
        return {figname: fig}
            

    
    def combine_heavytailedness_results(self, L=20):
        rdir = "output/heavy_tailedness_%i/"%(L)
        for c in self.powlaw_controllers:
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
            np.savetxt(rdir+"heavy_tailedness_results_%s_%i_%s.txt"%(c,L,self.xminstr), data, header=header)

                    



if __name__ == "__main__":
    MAX_N = 500
    MIN_N = 300
    DELTA_N = 50
    N_SEEDS = 5
    L = 170
    nested = 'true'
    name = "xmin_range"
    pjotr = Plotter(MIN_N=MIN_N, MAX_N=MAX_N, DELTA_N=DELTA_N, N_SEEDS=N_SEEDS, xminstr=name)

    if "combine" in sys.argv:
        # First combine the data into seperate arrays 
        pjotr.combine_heavytailedness_results(L=L)

    
    # figs = pjotr.plot_heavytailedness(L=L)
    figs = pjotr.plot_search_efficiency(nested, L=L)
    # figs = pjotr.plot_targetfrequency(L=170)

    if "save" in sys.argv:
        figdir = "/home/johannes/projects/conferences/ECAI2020/paper/figures/"
        for name, fig in figs.items():
            fig.savefig(figdir+name+".pdf", bbox_inches='tight')
    else:
        plt.show()



    
        
