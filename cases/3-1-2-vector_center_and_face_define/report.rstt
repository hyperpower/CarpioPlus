Problem Description
===================

This cases is just a simple illustration of the definition of the structure vector located on the center of the cell. Every vector is defined by multiple the shared_ptr of Fields. The **VectorCenter** does not hold the data. It just has the shared_ptr of **Fields**. The data is store in **Fields**.

.. code:: c++

   template<St DIM>
   class SVectorCenter_{
      public:
      ...
      typedef SField_<DIM>             Field;
      typedef std::shared_ptr<Field> spField;
      ...

      protected:
         std::array<spField, DIM> _arrs;

         ...
   };

The **VectorFace** is for holding the data on the Cell faces. Unlike **VectorCenter**, **VectorFace** stores all the data. The pseudo code is:


.. code:: c++

   template<St DIM>
   class SVectorFace_{
   protected:
      spGrid  _grid;
      spGhost _ghost;
      spOrder _order;

      Mat _mat[DIM]; // store the face data

      ...
   };


Vector Center
===================

1D Vector Center
-------------------
We have One-dimension field from x = 0 to x = 1.5. There are 5 control volumes.

.. figure:: _static/{{folder_name}}/vc_1d.png
   :alt: One dimension vector center
   :align: center 

   One dimension vecter center.

2D Vector Center
-------------------
The 2D field is defined in [0.0, 1.5] :math:`\times` [0.0, 1.5]. The mesh has 25 control volumes (5 :math:`\times` 5). The intial values for the field are difined as:

.. math::
   
   u &= \sin(x) \\
   v &= \sin(y)

A vector field is shown in following figure.

.. figure:: _static/{{folder_name}}/vc_2d.png
   :alt: Two dimension vector center
   :align: center 

   Two dimension vector center.

3D Vector Center
--------------------
The 3D vector center is defined in [0.0, 1.5] :math:`\times` [0.0, 1.5] :math:`\times` [0.0, 1.5]. The mesh has 25 control volumes (5 :math:`\times` 5 :math:`\times` 5). The vector center field is defined as: 

.. math::
   
   u &= \sin(x) \\
   v &= \sin(y) \\
   w &= \sin(z)

.. figure:: _static/{{folder_name}}/vc_3d.png
   :alt: Three dimension vector center
   :align: center 

   Three dimension vector center.

Vector Face
===================

1D Vector Face
-------------------
We have One-dimension field from x = 0 to x = 1.5. There are 5 control volumes. The vector face comes from vector center. The boundary condition has been imposed. :math:`x=0, u= 0.3` and :math:`x = 1.5`, :math:`u = 0.5`.

.. math::
   
   u = \sin(x)


.. figure:: _static/{{folder_name}}/vf_1d.png
   :alt: One dimension vector center
   :align: center 

   One dimension vector face.

2D Vector Face
-------------------
The 2D vector face is defined in [0.0, 1.5] :math:`\times` [0.0, 1.5]. The mesh has 25 control volumes (5 :math:`\times` 5). The intial values for the field are difined as:

.. math::
   
   u &= \sin(x + y) \\
   v &= \sin(x + y)

The boundary condition has been added. :math:`x=0, u= 0.3` and :math:`x = 1.5`, :math:`u = 0.5`. The Neuman boundary condition is implied in y direction (if :math:`y = 0` or :math:`y = 1.5`, :math:`\frac{\partial u}{\partial y} = 0`)

.. figure:: _static/{{folder_name}}/vf_2d.png
   :alt: Two dimension vector face
   :align: center 

   Two dimension vector face. 

.. figure:: _static/{{folder_name}}/vf_2d3.png
   :alt: Two dimension vector face
   :align: center 

   Two dimension vector face in 3D.