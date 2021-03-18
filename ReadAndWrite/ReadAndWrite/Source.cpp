#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <vector>
#include <queue>
#include <ctime>

std::vector<std::thread> readerThreads;
std::vector<std::thread> writerThreads;

int bosshealth = 100;
int numReaders = 0;
int numWriters = 0;
std::mutex lock;

void Reader(int t_readerID)
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));

		if (numWriters <= 0)
		{
			lock.lock();
			numReaders = numReaders + 1;
			lock.unlock();
			
			std::cout << "ReaderID:" + std::to_string(t_readerID) << ": Boss' health: " + std::to_string(bosshealth) << std::endl;
			
			lock.lock();
			numReaders = numReaders - 1;
			lock.unlock();
		}
	}
}

void Writer(int t_writerID)
{
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		if (numReaders <= 0 && numWriters <= 0)
		{
			lock.lock();
			numWriters = numWriters + 1;
			lock.unlock();
			
			std::cout << "Player:" + std::to_string(t_writerID) << " attacks" << std::endl;

			lock.lock();
			bosshealth--;
			numWriters = numWriters - 1;
			lock.unlock();
		}
	}
}

int main()
{
	srand(time(nullptr));

	for (int i = 0; i < 25; i++)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::thread read(Reader, i);
		readerThreads.push_back(move(read));

		std::thread write(Writer, i);
		writerThreads.push_back(move(write));
	}

	for (int i = 0; i < 25; i++)
	{
		readerThreads.at(i).join();
		writerThreads.at(i).join();
	}

	std::cin.get();
	return 0;
}