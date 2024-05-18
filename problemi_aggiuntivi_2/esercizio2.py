from matplotlib import pyplot as plt
plt.rcParams['figure.figsize'] = (12,8)
plt.rc('xtick',labelsize=15)
plt.rc('ytick',labelsize=15)
plt.rc('legend',fontsize=15)
plt.rc('font',size=15)

import numpy as np
from matplotlib import pyplot as plt
from scipy.integrate import simpson
from scipy import integrate 
from scipy.stats import chi2,norm
from scipy.optimize import minimize
np.random.seed(276884)
#np.random.seed(10)

def model(x,pars):
    a = pars[0]
    #theta = pars[1]
    theta = np.pi/2.-0.3
    #m0 = 10
    m0 = pars[1]
    gamma = 1
    xint = np.linspace(5,18,10001)
    val_model = lambda x: np.abs(1+a*np.exp(1j*theta)/((x**2-(m0)**2)+1j*m0*gamma))**2
    if (type(a) is not np.ndarray) and type(theta) is not np.ndarray:
        integral = simpson(val_model(xint),xint)
        return val_model(x)/integral
    else:
        a = a.reshape(-1,1)
        theta = theta.reshape(-1,1)
        integral = simpson(val_model(xint),xint,axis=1).reshape(-1,1)
        return val_model(x)/integral    
    #return 1+(np.power(a,2)+2*a*np.sin(theta)*m0*gamma+2*a*(np.power(x,2)-np.power(m0,2))*np.cos(theta))/( np.power(np.power(x,2)-np.power(m0,2),2)+np.power(m0*gamma,2) )
    
def generate_data(a, m0):
    x = np.random.uniform(5,18,1000000)
    y = model(x,[a, m0])
    return x[np.random.uniform(0,np.max(y),1000000)<y][:ndata]

def nll(data,model,pars):
    log = np.log(model(data,pars))
    if len(log.shape)==1: return -np.sum(np.log(model(data,pars)))
    else: return -np.sum(np.log(model(data,pars)),axis=1)

ndata = 300

thetaval = np.pi/2.-0.3
m0val = 10
aval = 5
x = np.linspace(5,18,101)
ymodel = model(x,[aval, m0val])

data = generate_data(aval, m0val)
len(data)
nbins = 20
plt.plot(x,ymodel*ndata*(x.max()-x.min())/nbins,label='model')
plt.hist(data,bins=np.linspace(x.min(),x.max(),nbins+1),histtype='step',label='data')
plt.xlabel('m [a.u.]')
plt.ylabel('count')
plt.legend(loc='best')
plt.savefig('data_model.png')
#plt.show()

#fit_result = minimize(lambda pars: nll(data,model,pars),x0=[0,thetaval], bounds=[(0,0),(-np.inf,np.inf)])

## p-value scan for null hypothesis
step = 0.1
n_point = int((18-5)/step)
mbar = np.linspace(5,18,n_point+1)
p_val = np.array([])
t = np.array([])
std_dev = np.array([])
for m in mbar:
    fit_result = minimize(lambda pars: nll(data,model,pars),x0=[aval,m], bounds=[(-np.inf,np.inf),(m,m)])
    #print(fit_result.x)
    t = np.append(t, 2*(nll(data, model, pars=[0,0,0]) -fit_result.fun) ) 
    #print(m)
    #print('t = %.5f'%t)
    p_val = np.append(p_val, 1 - chi2(1).cdf(t[-1]) )
    
    #x = np.linspace(5,18,101)
    #plt.hist(data,histtype='step',bins=np.linspace(x.min(),x.max(),nbins+1),label='data')
    #plt.plot(x,model(x,pars=[aval,thetaval, m0val])*ndata*(x.max()-x.min())/nbins,label='model generated')
    #plt.plot(x,model(x,pars=fit_result.x)*ndata*(x.max()-x.min())/nbins,label='model fitted')
    #plt.legend(loc='best')
    #plt.xlabel('m [a.u.]')
    #plt.ylabel('count')
    #plt.show()
    #print(p_val[-1])

print('min of p-value = %f '%p_val.min())
print('min at m_0 = %f'%mbar[p_val.argmin()])
plt.figure()
p = plt.plot(mbar,p_val,label= r'p-value $H_0$', lw=1)
plt.xlabel(r'$\overline{m_0}$',fontsize=15)
plt.ylabel('p-value',fontsize=15)
plt.axhline(0.05,linestyle='dashed',color='red')
plt.text(15,0.05,r'5%',ha='center', va='center',backgroundcolor='white')
plt.legend(loc='best')
plt.title('p-value scan for the null hypothesis')
#plt.savefig('p_value_m.png')
#plt.show()

plt.figure()
std_dev =  norm.ppf(1-p_val/2)
print('max of sigma = %.5f (2-sided)'%std_dev.max())
plt.plot(mbar,std_dev,label= r'$\sigma$ for $H_0$', lw=1)
#plt.plot(mbar,np.sqrt(t),label= r'$\sigma$ for $H_0$', lw=1)    same as above
plt.xlabel(r'$\overline{m_0}$',fontsize=15)
plt.ylabel(r'$\sigma$',fontsize=15)
plt.axhline(3,linestyle='dashed',color='red')
plt.text(17,3,r'3$\sigma$',ha='center', va='center',backgroundcolor='white')
plt.axhline(2,linestyle='dashed',color='red')
plt.text(17,2,r'2$\sigma$',ha='center', va='center',backgroundcolor='white')
plt.axhline(1,linestyle='dashed',color='red')
plt.text(17,1,r'1$\sigma$',ha='center', va='center',backgroundcolor='white')
plt.legend(loc='best')
plt.title('Standard deviation scan for the null hypothesis')
#plt.savefig('sigma_m.png')
#plt.show()


fit = minimize(lambda pars: nll(data,model,pars),x0=[aval,m0val])
print(fit)
ntoys = 10000
t_null = np.array([])
t_obs = -2*( fit.fun - nll(data,model,pars=[0,0]))
for i in range(ntoys):
    toy_data_bkg = generate_data(a=0, m0 = 0)
    #result_bkg = minimize(lambda pars: nll(toy_data_bkg,model,pars),x0=[0,0,0])    
    result_bkg = minimize(lambda pars: nll(toy_data_bkg,model,pars),x0=[0,0])
    #print(result_bkg.x)
    t_null = np.append(t_null, -2*(result_bkg.fun-nll(toy_data_bkg,model,pars=[0,0,0])) )
    #plt.figure()
    #plt.hist(toy_data_bkg,bins=np.linspace(x.min(),x.max(),nbins+1),histtype='step',label=r'data $H_0$ ')
    #plt.plot(x,model(x,pars=result_bkg.x)*ndata*(x.max()-x.min())/nbins,label='model fitted')
    #plt.plot(x,model(x,pars=[0,thetaval, 0])*ndata*(x.max()-x.min())/nbins,label='model generated')
    #plt.legend(loc='best')
    print(i)

plt.figure()
nbins = 50
_,bins,_=plt.hist(t_null,bins=nbins,histtype='step',label='null hypothesis')
x = np.linspace(bins[1:].min(),bins.max(),101)
plt.xlabel('test statistic')
mean, loc, scale = chi2.fit(t_null)
print('mean of t distrib = %5f.'%mean)
plt.plot(x,chi2(1).pdf(x)*ntoys*(bins.max()-bins.min())/nbins,label='$\chi^2$ 1 dof')
plt.plot(x,chi2(2).pdf(x)*ntoys*(bins.max()-bins.min())/nbins,label='$\chi^2$ 2 dof')
plt.plot(x,chi2(mean).pdf(x)*ntoys*(bins.max()-bins.min())/nbins,label='$\chi^2$ fitted')
plt.axvline(t_obs,color='black',label='$t_{obs}^\prime$')
plt.legend(loc='best')
plt.ylabel('count')
plt.title('Distribution of test statistic for null hypothesis')
plt.savefig('t_distrib.png')

pvalue = len(t_null[t_null>=t_obs])/len(t_null)
print('t_obs = %f'%t_obs)
print('p-value = %f'%pvalue)
print('significance = %f 1-sided'%norm.ppf(1-pvalue))
plt.show()