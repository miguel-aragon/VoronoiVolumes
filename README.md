# VoronoiVolumes

## Description

Several codes to compute Voronoi 3D distributions from a set of Voronoi seeds. The motivation to write this code was the need of simple test cases for pattern recognition algorithms I developed to study the Large Scale Structure of the Universe.

The codes takes an input datacube and finds the distance from each voxel in the datacube to its closest Voronoi seed. While this brute-force is in principle innefficient in practice the Voronoi seeds set is quite small. A 512<sup>3</sup> cube with 100 Voronoi seeds takes a few seconds to run.

The following papers have used Voronoi fields produced with these codes (259 citations in total (2018)):

* Classifying the Large Scale Structure of the Universe with Deep Neural Networks http://adsabs.harvard.edu/abs/2018arXiv180400816A

* Star Formation Isochrone Surfaces: Clues on Star Formation Quenching in Dense Environments http://adsabs.harvard.edu/abs/2014arXiv1412.1119A

* The Spine of the Cosmic Web http://adsabs.harvard.edu/abs/2010ApJ...723..364A

* Unfolding the hierarchy of voids http://adsabs.harvard.edu/abs/2010MNRAS.404L..89A

* The multiscale morphology filter: identifying and extracting spatial patterns in the galaxy distribution http://adsabs.harvard.edu/abs/2007A%26A...474..315A




