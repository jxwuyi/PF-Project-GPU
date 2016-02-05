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
    theta ~ uniform(-1.0, 1.0)
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
  sub proposal_parameter {
    theta ~ truncated_gaussian(theta, 0.03, lower = -1.0, upper = 1.0)
  }
}
