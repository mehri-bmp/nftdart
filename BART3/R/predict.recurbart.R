
## BART: Bayesian Additive Regression Trees
## Copyright (C) 2017 Robert McCulloch and Rodney Sparapani

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

predict.recurbart <- function(object, newdata,
                              mc.cores=getOption('mc.cores', 1L),
                              openmp=(mc.cores.openmp()>0), ...) {

    ##if(class(newdata) == "data.frame") newdata=bartModelMatrix(newdata)
    ##if(class(newdata) != "matrix") stop("newdata must be a matrix")

    p <- length(object$treedraws$cutpoints)

    if(p!=ncol(newdata))
        stop(paste0('The number of columns in newdata must be equal to ', p))

    if(.Platform$OS.type == "unix") mc.cores.detected <- detectCores()
    else mc.cores.detected <- NA

    if(!is.na(mc.cores.detected) && mc.cores>mc.cores.detected) mc.cores <- mc.cores.detected

    if(.Platform$OS.type != "unix" || openmp || mc.cores==1) call <- recur.pwbart
    else call <- mc.recur.pwbart

    if(length(object$binaryOffset)==0) object$binaryOffset=object$offset

    return(call(newdata, object$treedraws, mc.cores=mc.cores,
                binaryOffset=object$binaryOffset,
                type=object$type, ...))
}

