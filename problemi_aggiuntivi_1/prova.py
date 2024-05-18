from scipy.stats import poisson,norm
from math import log
from scipy import integrate
#from scipy.integrate import simpson
from scipy.optimize import minimize
from functools import partial
from scipy.integrate import quad
import numpy as np
from matplotlib import pyplot as plt
#from scipy.integrate import cumulative_trapezoid
from scipy.stats import chi2
from scipy.stats import norm

plt.rcParams['figure.figsize'] = (12,8)
plt.rc('xtick',labelsize=15)
plt.rc('ytick',labelsize=15)
plt.rc('legend',fontsize=15)
plt.rc('font',size=15)


def model_poisson(n,s,b):
    return poisson(s+b).pmf(n)

def constraint_b(b,bmu,sigmab):
    return norm(loc=bmu,scale=sigmab).pdf(b)

def model_constraint_freq(n,s,b,bbar,sigmab):
    return model_poisson(n,s,b)*constraint_b(bbar,b,sigmab)

def model_constraint_2d_freq(n,x,bbar,sigmab):
    s = x[0]
    b = x[1]
    return model_constraint_freq(n,s,b,bbar,sigmab)

def testStatistic(model,n,s,b,bestNLL,**kwargs):
    return 2*(nll(model=model,n=n,s=s,b=b,**kwargs)-bestNLL)

def nll_2d(model,n,x,**kwargs):
    return -np.log(model(n=n,x=x,**kwargs))

def nll(model,n,s,b,**kwargs):
    return -np.log(model(n,s,b,**kwargs))

s = 150
b= 50
sigmab = 30

#limit = chi2(1).ppf(0.8) # 90% one sided CL
limit = pow(norm.ppf(0.95),2)
print(limit)
n_point = 200
s_scan = np.linspace(100,230, n_point)
upper_limit = np.array([])
truth = 0
N = 1000
s_dist = np.array([])

for i in range(N):
    bbar = np.random.normal(b, sigmab)
    n_i = np.random.poisson(s+b)

    res = minimize(partial(nll_2d,model_constraint_2d_freq,n_i,bbar=bbar,sigmab=sigmab),x0=(150,50),bounds=[(0,np.inf),(0,np.inf),])  #fit of s & b given n_i & bbar
    s_dist = np.append(s_dist,res.x[0])
    print(i)

nbins = 20
plt.hist(s_dist, bins = np.linspace(70,260, 25))
#data,bins,_=plt.hist(np.arange(nbins),s_dist,label='data',bins=np.arange(nbins+1),histtype='step')
plt.legend(loc='best')
plt.xlabel('a.u.')
plt.ylabel('counts')
plt.show()