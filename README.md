# BezierPaths

This project uses Bezier curves o arbitrary order to generate a smooth and controlled path that an object can follow. The goal is to allow the object to move along this path with natural and precise motion, using the properties of Bezier curves to define the direction and shape of the trajectory at each point.


## DEFINITION OF BEZIER CURVES: 
"Bezier curves are parametric curves defined by a set of control points, with each point influencing the final shape of the curve. The number of control points determines the order of the curve.


In the case of a first-order Bezier curve, it is defined by the formula:

$$
B(t) = (1 - t) P_0 + t P_1 , \quad 0 < t < 1
$$

This represents a simple linear interpolation between two points, where the parameter \( t \) controls the position on the Bezier curve, varying from 0 to 1. At \( t = 0 \), the curve corresponds to the initial control point \( P_0 \), and at \( t = 1 \), the curve reaches the final control point \( P_n \). Throughout the intermediate values of \( t \), the curve moves smoothly between these control points.

For a second-degree Bezier curve, the formula becomes:

$$
B(t) = (1 - t) \left((1 - t) P_0 + t P_1 \right) + t \left((1 - t) P_1 + t P_2 \right), \quad 0 < t < 1
$$

In this case, we first calculate the intermediate points \( L_0 \) and \( L_1 \) by linear interpolation between \( P_0 \) and \( P_1 \), and between \( P_1 \) and \( P_2 \), respectively. Then, we find the point on the curve \( B(t) \) by interpolating between \( L_0 \) and \( L_1 \), based on the parameter \( t \). For higher-order Bezier curves, the formula is generalized using Bernstein polynomials, so that a curve of order \( n \) is described by the relation:

$$
B(t) = \sum_{i=0}^{n} \binom{n}{i} (1 - t)^{n-i} t^i P_i, \quad 0 < t < 1
$$

where \( P_i \) represents the control points, and the first term in the sum is a binomial coefficient.

## IMPLEMENTATION
The creation of Bezier curves is done through two functions in the `Curve` class: `CreateCurve()` and `DrawCurve()`. The `CreateCurve()` function handles populating the necessary buffers for representing the curve, while `DrawCurve()` is responsible for the actual drawing of the curve.


<div style="display: flex; align-items: flex-start;">
    <div style="margin-right: 20px;">
        <pre class="col-md-6 col-md-offset-3">
if (t <= 1.0f)
{
    glm::vec2 point(0.0f, 0.0f);
    for (int i = 0; i < n; i++)
    {
        float bernsteinPoly = BinomC(n - 1, i) * std::pow(t, i) * std::pow(1 - t, n - 1 - i);
        point += controlPoints[i] * bernsteinPoly;
    }
    curveStrip.push_back(point);
    
    glBindBuffer(GL_ARRAY_BUFFER, curveVBO.GetID());
    glBufferSubData(GL_ARRAY_BUFFER, 0, curveStrip.size() * sizeof(glm::vec2), curveStrip.data());

    t += dt;  // t forward for the next frame
}
        </pre>
    </div>
    <div>
        <img src="BezierPaths/Resources/BezierPaths2.gif" alt="Bezier Curve" style="max-width: 300px;">
    </div>
</div>

|  | |
|---|---|
|  ![](BezierPaths/Resources/BezierPaths.gif) | ![](BezierPaths/Resources/bezierPic1.png) |
|  ![](BezierPaths/Resources/BezierPaths_1.gif) | ![](BezierPaths/Resources/BezierPaths2.gif)|
|  ![](BezierPaths/Resources/BezierPathsRect1.gif) | ![](BezierPaths/Resources/BezierPathsRect2.gif) |


 if (t <= 1.0f)
 {

     glm::vec2 point(0.0f, 0.0f);

     for (int i = 0; i < n; i++)
     {
         float bernsteinPoly = BinomC(n - 1, i) * std::pow(t, i) * std::pow(1 - t, n - 1 - i);
         point += controlPoints[i] * bernsteinPoly;

     }
     curveStrip.push_back(point);
    
    glBindBuffer(GL_ARRAY_BUFFER, curveVBO.GetID());
    glBufferSubData(GL_ARRAY_BUFFER, 0, curveStrip.size() * sizeof(glm::vec2), curveStrip.data());


    t += dt;  // t forward for the next frame
}