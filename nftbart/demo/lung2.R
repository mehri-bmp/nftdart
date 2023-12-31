
library(nftbart)

B=8
options(mc.cores=B)

data(lung)
N=length(lung$status)

##lung$status: 1=censored, 2=dead
##delta: 0=censored, 1=dead
delta=lung$status-1

## this study reports time in days rather than weeks or months
times=lung$time
times=times/7  ## weeks

## matrix of covariates
x.train=cbind(lung[ , -(1:3)])
## lung$sex:        Male=1 Female=2

set.seed(99)
post=nft2(x.train, x.train, times, delta, K=0)

x.test = rbind(x.train, x.train)
x.test[ , 2]=rep(1:2, each=N)
K=100
events=c(0, quantile(times[delta==1], (0:(K-1))/(K-1)))
a=proc.time()
pred = predict(post, x.test, x.test, K=K, events=events[-1],
               XPtr=TRUE, FPD=TRUE)
print((proc.time()-a)/60)

plot(events, c(1, pred$surv.fpd.mean[1:K]), type='l', col=4,
     ylim=0:1, 
     xlab=expression(italic(t)), sub='weeks',
     ylab=expression(italic(S)(italic(t), italic(x))))
lines(events, c(1, pred$surv.fpd.upper[1:K]), lty=2, lwd=2, col=4)
lines(events, c(1, pred$surv.fpd.lower[1:K]), lty=2, lwd=2, col=4)
lines(events, c(1, pred$surv.fpd.mean[K+1:K]), lwd=2, col=2)
lines(events, c(1, pred$surv.fpd.upper[K+1:K]), lty=2, lwd=2, col=2)
lines(events, c(1, pred$surv.fpd.lower[K+1:K]), lty=2, lwd=2, col=2)
legend(90, 0.95, c('NFT BART', 'Mortality',
                     'Males', 'Females'), lwd=2, col=c(0, 0, 4, 2), lty=1)
abline(h=0:1, v=0)
