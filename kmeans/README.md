# Vector quantization with k-means

|Sushi (original)|Sushi K-means (NUM_ITERATIONS=1 and using cv::KMEANS_PP_CENTERS|
|:-:|:-:|
|![Sushi (original)](./assets/sushi.jpg)|![Sushi Kmeans](./assets/sushi-kmeans.jpg)|

> [❓] Using the kmeans.cpp program as an example, prepare an example program where the code is executed using the parameter nRodadas=1 and the centers are started randomly using the parameter KMEANS_RANDOM_CENTERS instead of KMEANS_PP_CENTERS. Perform 10 different rounds of the algorithm and compare the images produced. Explain why they can differ so much.

|1|2|3|4|5|
|:-:|:-:|:-:|:-:|:-:|
|![Sushi K-means (1)](./assets/sushi-kmeans-1.jpg)|![Sushi K-means (2)](./assets/sushi-kmeans-2.jpg)|![Sushi K-means (3)](./assets/sushi-kmeans-3.jpg)|![Sushi K-means (4)](./assets/sushi-kmeans-4.jpg)|![Sushi K-means (5)](./assets/sushi-kmeans-5.jpg)|

|6|7|8|9|10|
|:-:|:-:|:-:|:-:|:-:|
|![Sushi K-means (6)](./assets/sushi-kmeans-6.jpg)|![Sushi K-means (7)](./assets/sushi-kmeans-7.jpg)|![Sushi K-means (8)](./assets/sushi-kmeans-8.jpg)|![Sushi K-means (9)](./assets/sushi-kmeans-9.jpg)|![Sushi K-means (10)](./assets/sushi-kmeans-10.jpg)|

They may even look the same at a quick glance, but if you overlap each other during the execution of the algorithm, the changes will be noticeable. This may happen due to random initialization of cluster centers. In *K-means*, the initial position of the centers can influence the final result. With `NUM_ITERATIONS=1`, the algorithm does not have enough time to converge to an optimal solution, resulting in solutions that strongly depend on the initial positions of the centers. This can cause high variability in results between different runs of the algorithm.
