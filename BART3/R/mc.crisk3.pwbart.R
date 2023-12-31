
## BART: Bayesian Additive Regression Trees
## Copyright (C) 2017-2018 Robert McCulloch and Rodney Sparapani

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

mc.crisk3.pwbart <- function(
   x.test,		#x.test for cause 1
   x.test2,             #x.test for cause 2
   x.test3,             #x.test for cause 3
   treedraws,		#$treedraws for cause 1 from crisk.bart/mc.crisk.bart
   treedraws2,	        #$treedraws for cause 2
   treedraws3,	        #$treedraws for cause 3
   binaryOffset=0,	#mean to add on for cause 1
   binaryOffset2=0,	#mean to add on for cause 2
   binaryOffset3=0,	#mean to add on for cause 3
   mc.cores=getOption('mc.cores', 2L),
   type='pbart',
   transposed=FALSE,
   nice=19L
)
{
    if(.Platform$OS.type!='unix')
        stop('parallel::mcparallel/mccollect do not exist on windows')

    if(length(x.test)==0 | length(x.test2)==0 | length(x.test3)==0)
        stop('x.test, x.test2 and x.test3 must be provided')

    if(nrow(x.test)!=nrow(x.test2) & nrow(x.test)!=nrow(x.test3))
        stop('number of rows in x.test, x.test2 and x.test3 must be equal')

    if(!transposed) {
        x.test <- t(bartModelMatrix(x.test))
        x.test2 <- t(bartModelMatrix(x.test2))
        x.test3 <- t(bartModelMatrix(x.test3))
    }

    p <- length(treedraws$cutpoints)
    if(p!=nrow(x.test))
        stop(paste0('The number of columns in x.test must be equal to ', p))

    p <- length(treedraws2$cutpoints)
    if(p!=nrow(x.test2))
        stop(paste0('The number of columns in x.test2 must be equal to ', p))

    p <- length(treedraws3$cutpoints)
    if(p!=nrow(x.test3))
        stop(paste0('The number of columns in x.test3 must be equal to ', p))

    K <- ncol(x.test)
    k <- K%/%mc.cores
    j <- K
    print(paste0('Parallel predict with mc.cores=', mc.cores))
    for(i in 1:mc.cores) {
        if(i==mc.cores) h <- 1
        else h <- j-k

        parallel::mcparallel({psnice(value=nice);
            pwbart(x.test[ , h:j], treedraws, binaryOffset, 1, TRUE)},
            silent=(i!=1))
        j <- h-1
    }

    yhat.test.list <- parallel::mccollect()

    j <- K
    for(i in 1:mc.cores) {
        if(i==mc.cores) h <- 1
        else h <- j-k

        parallel::mcparallel({psnice(value=nice);
            pwbart(x.test2[ , h:j], treedraws2, binaryOffset2, 1, TRUE)},
            silent=(i!=1))
        j <- h-1
    }

    yhat.test2.list <- parallel::mccollect()

    j <- K
    for(i in 1:mc.cores) {
        if(i==mc.cores) h <- 1
        else h <- j-k

        parallel::mcparallel({psnice(value=nice);
            pwbart(x.test3[ , h:j], treedraws3, binaryOffset3, 1, TRUE)},
            silent=(i!=1))
        j <- h-1
    }

    yhat.test3.list <- parallel::mccollect()

    pred <- list()

    pred$binaryOffset <- binaryOffset
    pred$binaryOffset2 <- binaryOffset2
    pred$binaryOffset3 <- binaryOffset3
    x.test <- t(x.test)
    pred$tx.test <- x.test
    pred$tx.test2 <- t(x.test2)
    pred$tx.test3 <- t(x.test3)
    times <- unique(sort(x.test[ , 1]))
    pred$times <- times
    K <- length(times)
    pred$K <- K

    pred$yhat.test <- yhat.test.list[[1]]
    if(class(pred$yhat.test)[1]!='matrix') return(pred$yhat.test)

    pred$yhat.test2 <- yhat.test2.list[[1]]
    if(class(pred$yhat.test2)[1]!='matrix') return(pred$yhat.test2)

    pred$yhat.test3 <- yhat.test3.list[[1]]
    if(class(pred$yhat.test3)[1]!='matrix') return(pred$yhat.test3)

    if(mc.cores>1)
        for(i in 2:mc.cores) {
            pred$yhat.test <- cbind(pred$yhat.test, yhat.test.list[[i]])
            pred$yhat.test2 <- cbind(pred$yhat.test2, yhat.test2.list[[i]])
            pred$yhat.test3 <- cbind(pred$yhat.test3, yhat.test3.list[[i]])
        }

    H <- nrow(x.test)/K ## the number of different settings

    if(type=='pbart') {
        pred$prob.test <- pnorm(pred$yhat.test)
        pred$prob.test2 <- pnorm(pred$yhat.test2)
        pred$prob.test3 <- pnorm(pred$yhat.test3)
    }
    else if(type=='lbart') {
        pred$prob.test <- plogis(pred$yhat.test)
        pred$prob.test2 <- plogis(pred$yhat.test2)
        pred$prob.test3 <- plogis(pred$yhat.test3)
    }

    pred$surv.test <- (1-pred$prob.test)*(1-pred$prob.test2)*
        (1-pred$prob.test3)
    pred$cif.test <- pred$prob.test
    pred$cif.test2 <- (1-pred$prob.test)*pred$prob.test2
    pred$cif.test3 <- (1-pred$prob.test)*(1-pred$prob.test2)*pred$prob.test3

    for(h in 1:H)
        for(j in 2:K) {
            l <- K*(h-1)+j

            pred$cif.test[ , l] <- pred$cif.test[ , l-1]+
                pred$surv.test[ , l-1]*pred$cif.test[ , l]
            pred$cif.test2[ , l] <- pred$cif.test2[ , l-1]+
                pred$surv.test[ , l-1]*pred$cif.test2[ , l]
            pred$cif.test3[ , l] <- pred$cif.test3[ , l-1]+
                pred$surv.test[ , l-1]*pred$cif.test3[ , l]
            pred$surv.test[ , l] <- pred$surv.test[ , l-1]*
                pred$surv.test[ , l]
        }

    pred$cif.test.mean <- apply(pred$cif.test, 2, mean)
    pred$cif.test2.mean <- apply(pred$cif.test2, 2, mean)
    pred$cif.test3.mean <- apply(pred$cif.test3, 2, mean)
    pred$surv.test.mean <- apply(pred$surv.test, 2, mean)

    attr(pred, 'class') <- 'crisk3bart'

    return(pred)
}
