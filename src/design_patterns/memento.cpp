#include "../../include/design_patterns/memento.hpp"

Memento::Memento()
{

}

Memento::~Memento()
{

}

Memento::Snapshot Memento::save()
{
	Snapshot snapshot;
	_saveToSnapshot(snapshot);
	return snapshot;
}

void	Memento::load(const Memento::Snapshot& state)
{
	Snapshot snapshot = state;
	_loadFromSnapshot(snapshot);
}

Memento::Snapshot::Snapshot()
{

}

Memento::Snapshot::~Snapshot()
{

}