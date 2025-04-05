# libftpp
## Data Structures
### Pool
Manages a collection of reusable templated TType objects,
provided to the user via a Pool::Object class. This class
handles the acquirable pointer, and releases it back to the pool
when needed, calling the destructor of the TType object but
without deallocating the memory.

### DataBuffer
A polymorphic container for storing objects in byte format.

## Design Patterns
### Memento
Stores the current state of an object. Must be inherited by the
"saveable" class.

### Observer
An Observer class templated by TEvent, which allows users to
subscribe to events and be notified when those specifics events
are triggered.

### Singleton
Ensures that a templated TType class has only one instance
and provides a way to access it.

### StateMachine
A StateMachine class templated by TState, managing transi-
tions between states.

## IOStream
### ThreadSafeIOStream
Thread-safe version of iostream.

## Threads
### ThreadSafeQueue
A templated TType thread-safe version of a queue.

### Thread
A wrapper for the std::thread, with a name.

### WorkerPool
Manages worker threads to execute jobs. It contain a subclass
IJobs, which describes interface for a job to be executed by
the worker pool.

### PersistentWorker
A thread that continuously performs a set of tasks defined by
users.

## Network
### Message
Handles messages between client and server.

### Client
Client-side networking.

### Server
Server-side networking.

## Mathematics
### IVector2
A 2D vector with a templated type.

### IVector3
A 3D vector with a templated TType.

### Random2DCoordinateGenerator
Generates pseudo-random numbers based on 2D coordinates.

### PerlinNoise2D
Generates 2D Perlin noise.
