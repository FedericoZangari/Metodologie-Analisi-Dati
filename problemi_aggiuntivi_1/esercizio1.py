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
s = 5
b= 15
sigmab = 9.5

#limit = chi2(1).ppf(0.8) # 90% one sided CL
limit = pow(norm.ppf(0.95),2)
print(limit)
n_point = 400
s_scan = np.linspace(70,250, n_point)
s_scan = np.linspace(0,35, n_point)
#print(s_scan)
upper_limit = np.array([])
truth = 0
N = 1000

for i in range(N):
    bbar = np.random.normal(b, sigmab)
    n_i = np.random.poisson(s+b)
    #print('bbar =%d'%bbar)
    #print('n_i =%d'%n_i)
    #res = minimize(partial(nll_2d,model_constraint_2d_freq,n_i,bbar=bbar,sigmab=sigmab),x0=(150,50),bounds=[(0,np.inf),(0,np.inf),])  #fit of s & b given n_i & bbar
    res = minimize(partial(nll_2d,model_constraint_2d_freq,n_i,bbar=bbar,sigmab=sigmab),x0=(5,15),bounds=[(0,np.inf),(0,np.inf),])  #fit of s & b given n_i & bbar
    #res = minimize(partial(nll_2d,model_constraint_2d_freq,n_i,bbar=bbar,sigmab=sigmab),x0=(5,15),bounds=[(-100,np.inf),(-100,np.inf),])  #fit of s & b given n_i & bbar
    nnl_fit = res.fun  #nnl at ML estimators
    print(res.x)
    print(-2*nnl_fit)
    t = np.array([])
    for si in s_scan:
        res = minimize(partial(nll,model_constraint_freq,n_i,si,bbar=bbar,sigmab=sigmab),x0=(15,),bounds=[(0,np.inf),])  # fit of b with s fixed 
        #res = minimize(partial(nll,model_constraint_freq,n_i,si,bbar=bbar,sigmab=sigmab),x0=(15,),bounds=[(-100,np.inf),])  # fit of b with s fixed 
        nnl_si = res.fun
        t = np.append(t,2*(nnl_si - nnl_fit))
        #print('t = %.5f'%t)
        #t = testStatistic(model=model_constraint_freq,n=n_i,s=si,b=res.x[0],bestNLL=nnl_fit,bbar=bbar,sigmab=sigmab)  same as above
    idx_min = t.argmin()
    print('idx_min = %d'%idx_min)
    print(t)
    t_new = np.array([])
    for j in range(n_point):
        if j > idx_min:
            t_new = np.append(t_new,t[j])
        else:
            t_new = np.append(t_new,0)

    print(t_new> limit)
    idx = (t_new> limit).argmax()
    print(idx)
    upper_limit = np.append(upper_limit,0.5*(s_scan[idx]+s_scan[idx-1]))
    print('upper lim: %.2f' %upper_limit[i])
    p = plt.plot(s_scan,t,label=r'$\sigma_b=%.2f$'%sigmab, lw=1)
    plt.xlabel('s',fontsize=15)
    plt.ylabel('t',fontsize=15)
    plt.legend(loc='best')
    plt.vlines(upper_limit[i],0,limit,lw=1,color=p[0].get_color())
    plt.axhline(limit,color='black',lw=1)
    plt.show()
    
    if upper_limit[i] > s :
        truth += 1
    print(i)
frac = truth/N
print('fraction = %.3f'%frac)
#plt.hist(upper_limit, bins = np.linspace(70,250, 30),label=r'Distribution of $s_{up}$')
plt.hist(upper_limit, bins = np.linspace(-20,30, 40),label=r'Distribution of $s_{up}$')
plt.axvline(s,label=r'$s_{true}$',color='red',linewidth=1)
plt.title('Distribution of upper limit on $s$')
plt.xlabel(r'$s_{up}$')
plt.ylabel('counts')
plt.legend(loc='best')
plt.savefig('upper_lim_distrib_3.png')
plt.show()