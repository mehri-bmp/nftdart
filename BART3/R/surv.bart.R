
## BART: Bayesian Additive Regression Trees
## Copyright (C) 2017-2020 Robert McCulloch and Rodney Sparapani
## surv.bart.R

## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2 of the License, or
## (at your option) any later version.

## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.

## You should have received a copy of the GNU General Public License
## along with this program; if not, a copy is available at
## https://www.R-project.org/Licenses/GPL-2


surv.bart <- function(
    x.train = matrix(0,0,0),
    y.train=NULL, times=NULL, delta=NULL,
    x.test = matrix(0,0,0),
    K=NULL, events=NULL, ztimes=NULL, zdelta=NULL,
    sparse=FALSE, theta=0, omega=1,
    a=0.5, b=1, augment=FALSE, rho=0, grp=NULL,
    xinfo=matrix(0,0,0), usequants=FALSE,
    ## cont=FALSE,
    rm.const=TRUE, type='pbart',
    ntype=as.integer(
        factor(type, levels=c('wbart', 'pbart', 'lbart'))),
    k = 2, ## BEWARE: do NOT use k for other purposes below
    power = 2, base = 0.95,
    ##impute.mult=NULL, impute.prob=NULL, impute.miss=NULL,
    offset = NULL, tau.num=c(NA, 3, 6)[ntype],
    ##binaryOffset = NULL,
    ntree = 50L, numcut = 100L,
    ndpost = 1000L, nskip = 250L, keepevery = 10L,
    ##nkeeptrain=ndpost, nkeeptest=ndpost,
    ##nkeeptreedraws=ndpost,
    printevery=100L,
    ##keeptrainfits=TRUE,
    id = NULL,     ## only used by surv.bart
    seed = 99L,    ## only used by mc.surv.bart
    mc.cores = 2L, ## ditto
    nice=19L       ## ditto
)
{
    ## multinomial imputation does not appear to be well suited
    ## time-to-event outcomes with discrete time method
    impute.mult=NULL
    impute.prob=NULL
    impute.miss=NULL

    if(is.na(ntype) || ntype==1)
        stop("type argument must be set to either 'pbart' or 'lbart'")

    ## x.train <- bartModelMatrix(x.train)
    ## x.test <- bartModelMatrix(x.test)

    ## if(length(rho)==0) rho=ncol(x.train)

    impute = length(impute.mult)
    if(impute==1)
        stop("The number of multinomial columns must be greater than 1\nConvert a binary into two columns")

    ## cold = CDimpute(x.train, x.test, impute.mult)
    ## x.train = cold$x.train
    ## x.test = cold$x.test

    if(length(y.train)==0) {
        ## times.=times
        ## times.[delta==0]=times.[delta==0]+max(times[delta==1])
    
        ## hot = HDimpute(x.train, times., x.test)
        ## x.train = hot$x.train
        ## x.test = hot$x.test

        pre <- surv.pre.bart(times, delta, x.train, x.test, K=K,
                             events=events, ztimes=ztimes, zdelta=zdelta,
                             rm.const=rm.const, numcut=numcut, grp=grp, 
                             xinfo=xinfo, usequants=usequants)

        y.train <- pre$y.train
        x.train <- t(pre$tx.train)
        x.test  <- t(pre$tx.test)
        xinfo = pre$xinfo
        grp = pre$grp
        rho = sum(1/pre$grp)
        numcut = pre$numcut

        times   <- pre$times
        K       <- pre$K

        if(impute>1) {
            N = length(y.train)
            y.train = y.train[N:1] ## backwards
            x.train = x.train[N:1, ]
            impute.mult=impute.mult+1 ## move columns over by 1 for t
            pre=surv.pre.bart(times, delta, impute.prob, K=K, events=events)
            impute.prob=pre$tx.train[N:1, -1]
            impute.miss=pre$tx.train[ , 1]*0
            for(j in 1:impute) {
                i=impute.mult[j]
                impute.miss = pmax(impute.miss, is.na(x.train[ , i]))
            }
            impute.mask=impute.miss
            j=c(1, which(x.train[ , 1]==pre$times[1])+1)
            impute.mask[j[-length(j)]] = 0 ## the last j is N+1
            ##impute.mask=(pre$tx.train[ , 1]>pre$times[1])*impute.miss
            impute.miss=impute.miss+impute.mask ## 1 impute, 2 retain
        }
        ##if(length(binaryOffset)==0) binaryOffset <- pre$binaryOffset
    }
    else {
        if(length(unique(sort(y.train)))>2)
            stop('y.train has >2 values; make sure you specify times=times & delta=delta')

        ##if(length(binaryOffset)==0) binaryOffset <- 0

        N = length(y.train)

        if(N==nrow(x.train)) {
            times <- unique(sort(x.train[ , 1]))
            x.train=t(x.train)
            x.test=t(x.test)
        } else {
            times <- unique(sort(x.train[1, ]))
        }
        K     <- length(times)

    }

    ##if(length(binaryOffset)==0) binaryOffset <- qnorm(mean(y.train))

    ## if(type=='pbart') call <- pbart
    ## else if(type=='lbart') {
    ##     ##binaryOffset <- 0
    ##     call <- lbart
    ## }

    post <- gbart(x.train=x.train, y.train=y.train,
                  x.test=x.test, type=type,
                  sparse=sparse, theta=theta, omega=omega,
                  a=a, b=b, augment=augment, rho=rho, grp=grp,
                  xinfo=xinfo, usequants=usequants,
                  ##cont=cont,
                  rm.const=rm.const,
                  k=k, power=power, base=base,
                  offset=offset, tau.num=tau.num,
                  impute.mult=impute.mult, impute.prob=impute.prob,
                  impute.miss=impute.miss,
                  ##binaryOffset=binaryOffset,
                  ntree=ntree, numcut=numcut,
                  ndpost=ndpost, nskip=nskip, keepevery=keepevery,
                  ##nkeeptrain=nkeeptrain, nkeeptest=nkeeptest,
                  ##nkeeptestmean=nkeeptestmean,
                  ##nkeeptreedraws=nkeeptreedraws,
                  printevery=printevery,
                  transposed=TRUE)

    if(type!=attr(post, 'class')) return(post)

    ##post$binaryOffset <- binaryOffset
    post$id <- id
    post$times <- times
    post$K <- K
    post$tx.train <- t(x.train)
    post$y.train <- y.train
    post$type <- type

    ## if(keeptrainfits) {
    ##      post$surv.train <- 1-post$prob.train

    ##      H <- nrow(x.train)/K ## the number of different settings

    ##      for(h in 1:H)
    ##          for(j in 2:K) {
    ##              l <- K*(h-1)+j

    ##              post$surv.train[ , l] <-
    ## post$surv.train[ , l-1]*post$surv.train[ , l]
    ##          }

    ##      post$surv.train.mean <- apply(post$surv.train, 2, mean)
    ## }

    if(length(x.test)>0) {
        post$tx.test <- t(x.test)
        H <- nrow(post$tx.test)/K ## the number of different settings

        post$surv.test <- 1-post$prob.test
        ## if(type=='pbart') post$surv.test <- 1-pnorm(post$yhat.test)
        ## else if(type=='lbart') post$surv.test <- 1-plogis(post$yhat.test)

        for(h in 1:H)
            for(j in 2:K) {
                l <- K*(h-1)+j

                post$surv.test[ , l] <-
                    post$surv.test[ , l-1]*post$surv.test[ , l]
            }

        post$surv.test.mean <- apply(post$surv.test, 2, mean)
    }

    attr(post, 'class') <- 'survbart'

    return(post)
}
