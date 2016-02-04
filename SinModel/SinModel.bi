/**
 * The Sin Model
 */
model SinModel {
  const sigma = 1  // time step (s)

  param theta     // parameter to estimate
  noise e        // noise, ml s**-1
  state X        // State
  obs Y          // Data

  sub parameter {
    theta ~ uniform(-5.0, 5.0)
  }

  sub initial {
    X ~ gaussian(0, 1.0)
  }

  sub transition {
    e ~ gaussian(0.0, 1.0)
    X <- sin(theta * X) + e
  }

  sub observation {
    Y ~ gaussian(X, 1.0)
  }
/*
  sub proposal_parameter {
    R ~ truncated_gaussian(R, 0.03, lower = 0.0)
    C ~ truncated_gaussian(C, 0.1, lower = 0.0)
    Z ~ truncated_gaussian(Z, 0.002, lower = 0.0)
    sigma2 ~ inverse_gamma(2.0, 3.0*sigma2)
  }
  */
}
