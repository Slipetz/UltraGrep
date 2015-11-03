#include <memory>

#include "WorkerFactory.h"
#include "DirectoryWorker.h"
#include "FileWorker.h"

void WorkerFactory::GenerateWorker(WorkerType const & fileType, path const& file)
{
	switch (fileType) {
	case WorkerType::DIRECTORY:
		threadPool->EnqueueWorker(std::move(unique_ptr<IWorker>(new DirectoryWorker(file, blueprint, *this))));
		break;
	case WorkerType::REGULARFILE:
		threadPool->EnqueueWorker(std::move(unique_ptr<IWorker>(new FileWorker(file, blueprint))));
		break;
	}
}
