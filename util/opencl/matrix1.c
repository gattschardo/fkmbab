// Multiply two matrices A * B = C
 
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <CL/cl.h>

#include "util.h"
 
#define MATRIXSIZE 12
#define WA MATRIXSIZE
#define HA MATRIXSIZE
#define WB MATRIXSIZE
#define HB WA
#define WC WB
#define HC HA
  
// Allocates a matrix with random float entries.
void randomInit(float* data, int size)
{
   for (int i = 0; i < size; ++i)
   data[i] = rand() / (float)RAND_MAX;
}
 
/////////////////////////////////////////////////////////
// Program main
/////////////////////////////////////////////////////////
 
int
main(int argc, char** argv)
{
 
   // set seed for rand()
   srand(2006);
 
   // 1. allocate host memory for matrices A and B
   unsigned int size_A = WA * HA;
   unsigned int mem_size_A = sizeof(float) * size_A;
   float* h_A = (float*) malloc(mem_size_A);
 
   unsigned int size_B = WB * HB;
   unsigned int mem_size_B = sizeof(float) * size_B;
   float* h_B = (float*) malloc(mem_size_B);
 
   // 2. initialize host memory
   randomInit(h_A, size_A);
   randomInit(h_B, size_B);
  
   // 3. print out A and B
   printf("Matrix A\n");
   for(int i = 0; i < size_A; i++)
   {
      printf("%f ", h_A[i]);
      if(((i + 1) % WA) == 0)
      printf("\n");
   }
   printf("\n");
 
   printf("Matrix B\n");
   for(int i = 0; i < size_B; i++)
   {
      printf("%f ", h_B[i]);
      if(((i + 1) % WB) == 0)
      printf("\n");
   }
 
   // 4. allocate host memory for the result C
   unsigned int size_C = WC * HC;
   unsigned int mem_size_C = sizeof(float) * size_C;
   float* h_C = (float*) malloc(mem_size_C);
 
   // 5. Initialize OpenCL
   cl_context clGPUContext;
   cl_command_queue clCommandQue;
   cl_program clProgram;
   cl_kernel clKernel;

   size_t dataBytes;
   size_t kernelLength;
   cl_int errcode;

   // OpenCl device memory for matrices
   cl_mem d_A;
   cl_mem d_B;
   cl_mem d_C;

   /*****************************************/
   /* Initialize OpenCL */
   /*****************************************/
   /* get platform info first */
   cl_uint numPlatforms;
   cl_platform_id platform = NULL;
   errcode = clGetPlatformIDs(0, NULL, &numPlatforms);
   if (errcode != CL_SUCCESS) {
   	printf("getting platform info failed: %d\n", errcode);
	return EXIT_FAILURE;
   }
   if (0 < numPlatforms) {
   	cl_platform_id *platforms = malloc(sizeof(cl_platform_id)*numPlatforms);
	errcode = clGetPlatformIDs(numPlatforms, platforms, 0);
	if (errcode != CL_SUCCESS) {
		printf("getting platform info failed: %d\n", errcode);
		return EXIT_FAILURE;
	}

	for (unsigned i = 0; i < numPlatforms; i++) {
		char pbuf[100];
		errcode = clGetPlatformInfo(platforms[i],
				CL_PLATFORM_VENDOR,
				sizeof(pbuf),
				pbuf,
				NULL);
		if (errcode != CL_SUCCESS) {
			printf("getting platform info failed: %d\n", errcode);
			return EXIT_FAILURE;
		}
		printf("\nfound platform (%d of %d): %s\n\n", i+1, numPlatforms, pbuf);
		platform = platforms[i];
	}

	free(platforms);
   }
   if (!platform) {
   	printf("Could not find platform!\n");
	return EXIT_FAILURE;
   }
   cl_context_properties cps[3] =
   {
   	CL_CONTEXT_PLATFORM,
	(cl_context_properties)platform,
	0
   };
   /* end retrieving platform info */
   clGPUContext = clCreateContextFromType(cps, CL_DEVICE_TYPE_GPU,
   		NULL, NULL, &errcode);
   /* error check */
   if (errcode < 0) {
	printf("creating context failed: %d\n", errcode);
   	return EXIT_FAILURE;
   }

   errcode = clGetContextInfo(clGPUContext,
   		CL_CONTEXT_DEVICES, 0, NULL,
		&dataBytes);
   cl_device_id *clDevices = (cl_device_id *)
   		malloc(dataBytes);
   errcode |= clGetContextInfo(clGPUContext,
   		CL_CONTEXT_DEVICES, dataBytes,
		clDevices, NULL);
   /* error check */
 
   //Create a command-queue
   clCommandQue = clCreateCommandQueue(clGPUContext,
   		clDevices[0], 0, &errcode);
   /* error check */

   // Setup device memory
   d_C = clCreateBuffer(clGPUContext,
   		CL_MEM_READ_WRITE,
		mem_size_A, NULL, &errcode);
   d_A = clCreateBuffer(clGPUContext,
   		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		mem_size_A, h_A, &errcode);
   d_B = clCreateBuffer(clGPUContext,
   		CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
		mem_size_B, h_B, &errcode);

   // 6. Load and build OpenCL kernel
   /* char *clMatrixMul = oclLoadProgSource("kernel.cl",
                        "// My comment\n", 
                        &kernelLength); */
   char *clMatrixMul = myLoadProgSource("kernel.cl", &kernelLength);
   /* error check */
 
   clProgram = clCreateProgramWithSource(clGPUContext, 
                1, (const char **)&clMatrixMul, 
                &kernelLength, &errcode);
   /* error check */
 
   errcode = clBuildProgram(clProgram, 0, 
              NULL, NULL, NULL, NULL);
   /* error check */
 
   clKernel = clCreateKernel(clProgram, 
               "matrixMul", &errcode);
   /* error check */
 
   // 7. Launch OpenCL kernel
   size_t localWorkSize[2], globalWorkSize[2];

   int wA = WA;
   int wC = WC;
   errcode = clSetKernelArg(clKernel, 0,
   		sizeof(cl_mem), (void *)&d_C);
   errcode |= clSetKernelArg(clKernel, 1,
   		sizeof(cl_mem), (void *)&d_A);
   errcode |= clSetKernelArg(clKernel, 2,
   		sizeof(cl_mem), (void *)&d_B);
   errcode |= clSetKernelArg(clKernel, 3,
   		sizeof(int), (void *)&wA);
   errcode |= clSetKernelArg(clKernel, 4,
   		sizeof(int), (void *)&wC);
   /* error check */

   localWorkSize[0] = localWorkSize[1] = MATRIXSIZE;
   globalWorkSize[0] = globalWorkSize[1] = MATRIXSIZE;

   errcode = clEnqueueNDRangeKernel(clCommandQue,
   		clKernel, 2, NULL, globalWorkSize,
		localWorkSize, 0, NULL, NULL);
   /* error check */

   // 8. Retrieve result from device
   errcode = clEnqueueReadBuffer(clCommandQue,
   		d_C, CL_TRUE, 0, mem_size_C,
		h_C, 0, NULL, NULL);
   /* error check */

   // 9. print out the results
   printf("Matrix C (Results)\n");
   for(int i = 0; i < size_C; i++)
   {
      printf("%f ", h_C[i]);
      if(((i + 1) % WC) == 0)
      printf("\n");
   }
   printf("\n");
 
   // 10. clean up memory
   free(h_A);
   free(h_B);
   free(h_C);

   clReleaseMemObject(d_A);
   clReleaseMemObject(d_B);
   clReleaseMemObject(d_C);

   free(clDevices);
   free(clMatrixMul);
   clReleaseContext(clGPUContext);
   clReleaseKernel(clKernel);
   clReleaseProgram(clProgram);
   clReleaseCommandQueue(clCommandQue);
}
