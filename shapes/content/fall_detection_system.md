---
title: "Fall Detection System"
date: 2020-12-07T10:53:24+01:00
draft: false

description: "Explanation of the basics about the fall detection system that is in development"
image: "fall_detection_system/fall_detector.jpg"
---

# Overview

In ARCO Research Group, in the context of the european project **H2020 Shapes**, we are working in a set of solutions to 
encourage the active ageing and improve the health condition at an advanced age. One of these solutions is a fall detection
system, that will allow us to detect automatically when a person has fell, in order to attend him as soon as possible. The
system here presented could be a very powerful and useful tool, taking into account that falls are the leading cause of 
accidental death.

{{% note "warning" %}}
All the information presented here is about an incomplete system that is in constant change, so some information could
be inaccurate.
{{% /note %}}

# Fundamentals

In order to build this system an `Intertial Measure Unit (IMU)` will be used, specifically a **MetamotionR** sensor, we spoke
about this sensor in a preovious [recipe](https://arcogroup.bitbucket.io/shapes/integrating_metamotionr_with_python/), 
where we explained how to use our library to extract different data from it. This IMU can give us information about
the movement with two 3-AXIS sensor, an accelerometer and a gyroscope, but the raw data by itself doesn't give enough information,
for this reason we must preprocess it to get the desired information. This sensor also counts with a magnetometer, that can be
useful to calculate the orientation of the sensor, considering the earth's magnetic field. In our case we choose the place the
sensor in the user's waist, because is the center of gravity of a person, and is sensitive to abrupt changes in the measurements 
of the IMU when a fall occurs. 

The algorithm developed for us is a mixture of the two type of algorithms used for fall detection, **Threshold algorithm** and
**Machine Learning Algorithm**, the thresholds algorithm are mainly used in low performance devices. Our threshold algorithm is 
a **Finite State Machine** that check if the inertial measures (mainly acceleration) exceed a threshold under certain circumstances, 
if this requisite is achieved, then a series of features is extracted from the raw data in a window os 1 second. With the previous 
features, a model is trained to detect if is a fall or not (in our case we are using **SVM**), so introducing the features extracted 
to our model we can know if they correspond to a fall. Why is needed a model? Well, this is a good question, and all is about the 
`Activities of Daily Living (ADL)`.

In a day, we made a lot of different activites that can be tracked by our imus, if we only use a threshold algorithm is possible
that a lot of activites that don't involve a fall can overpass the selected threshold, bringing to a fall positive. To avoid this 
scenary, a model trained previously with labeled data can help us to reduce this false positive. At the same time, the threshold
algoritm can act as a filter, in order to avoid a frequent extraction of features and evaluations by the model.

In the data collection, the most important ADL identified that can cause a fall positive are the following:

* Hitting the sensor
* Jumping
* Running
* Sitting in a chair or a sofa

To training the model, we have collected data from the previous ADLs and the following falls:

* Backward fall
* Left-side fall
* Right-side fall
* Front fall

All the falls have been made using a mattress, you may think that using a mattress could alter the data, but don't use it will 
alter it too, because the falls would be made with fear. We conclude that the best solution was to use a mattress because 
the data is less altered due to more natural falls.

# Training model

At this point, we are using only one subject for both training and testing. The environment where the data has being collected was
the ARCO Research Group's lab, located in the Technology and System Information Institute (ITSI). Following you can see the 
mattress used for the falls.

{{< polaroid src="mattress.jpg" caption="Mattress used to collect data" >}}

The activities carried out in the collection are these:

* 10x Hitting the sensor
* 10x Jumping
* 10x Running
* 10x Sitting on a chair or a sofa
* 5x Backward fall
* 5x Left-side fall
* 5x Right-side fall
* 5x Front fall

With this data, we have trained the SVM model that is going to be used for testing purpose.

# Testing algorithm

For testing purpose, training activities have been repeated in the following way:

* 3x Hitting the sensor
* 3x Jumping
* 3x Running
* 3x Sitting on a chair or a sofa
* 3x Backward fall
* 3x Left-side fall
* 3x Right-side fall
* 3x Front fall

In the next table you can see the results:

|       Ejercicio      | True Positive | True Negative | False Positive | False Negative |
|:-------------------- |:-------------:|:-------------:|:--------------:|:--------------:|
|  Hitting the sensor  |               |  3            |                |                |
|        Jumping       |               |  3            |                |                |
|        Running       |               |  3            |                |                |
|  Sitting on a chair  |               |  3            |                |                |
|     Front fall       |  3            |               |                |                |
|    Left-side fall    |  3            |               |                |                |
|    Right-side fall   |  3            |               |                |                |
|     Backward fall    |  2            |               |                |  1             |

As you can see, almost all the results are the expected, with the exception of one backward fall, which produce
a wrong result. You must to take into account that the tests were made with the same subject as training. Summarizing,
our system with the tests made has a reliability of **95,83%**. In future works, we are going to collect data from more
subjects, create a more complete battery of tests, and finally deploy this solutions in a real environment, monitoring
the residents from the nursing home El Salvador in order to test our system. 