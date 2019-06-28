#ifndef _TEST_MPI_LEARN_HPP_
#define _TEST_MPI_LEARN_HPP_

#include "gtest/gtest.h"
#include "utility/tinyformat.hpp"
#include <math.h>
#include <string>
#include <memory>
#include "mpi.h"

namespace carpio {

TEST(mpi, learnpi){
	int n, rank, size, i;
	double PI25DT = 3.141592653589793238462643;
	double mypi, pi, h, sum, x;

	MPI::Init();
	size = MPI::COMM_WORLD.Get_size();
	rank = MPI::COMM_WORLD.Get_rank();
	std::cout << rank << std::endl;

	n = 1000; // number of intervals

	MPI::COMM_WORLD.Bcast(&n, 1, MPI::INT, 0);
	h = 1.0 / (double) n;
	sum = 0.0;
	for (i = rank + 1; i <= n; i += size) {
		x = h * ((double) i - 0.5);
		sum += (4.0 / (1.0 + x * x));
	}
	mypi = h * sum;

	MPI::COMM_WORLD.Reduce(&mypi, &pi, 1, MPI::DOUBLE, MPI::SUM, 0);
	if (rank == 0)
		std::cout << "pi is approximately " << pi << ", Error is "
				<< fabs(pi - PI25DT) << std::endl;

//	MPI::Finalize();
}

TEST(mpi, p2p){

//	MPI::Init();
	auto size = MPI::COMM_WORLD.Get_size();
	auto rank = MPI::COMM_WORLD.Get_rank();
	if (rank == 0){
		std::cout << "M > " << "MPI size = " << size << std::endl;
		if(size == 1){
			std::cerr << "M > " << "MPI is not working" << std::endl;
			return;
		}
	}

	std::cout <<"S > " << "rank = " << rank << std::endl;

	int number;
	if (rank == 0) {
	    number = -1;
	    int dest = 1;
	    tfm::format(std::cout, "S%d> Send %d to S%d", rank, number, dest);
	    MPI::COMM_WORLD.Send(&number, 1, MPI_INT, dest, 0);
	} else if (rank == 1) {
		int source = 0;
	    MPI::COMM_WORLD.Recv(&number, 1, MPI_INT, source, 0);
	    tfm::format(std::cout, "S%d > Received number %d from S%d\n",
	           rank, number, source);
	}

	MPI::Finalize();
}


}

#endif
