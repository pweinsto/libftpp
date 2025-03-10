#ifndef STATE_MACHINE_HPP
# define STATE_MACHINE_HPP

# include <functional>
# include <set>
# include <map>
# include <optional>
# include <stdexcept>

template<typename TState>
class StateMachine
{
public:
	void addState(const TState& state)
	{
		m_states.insert(state);
		m_actions[state] = []{};

		if (!m_currentState.has_value()) 
			m_currentState = state;
	}

	void addAction(const TState& state, const std::function<void()>& lambda)
	{
		if (m_states.count(state) == 0)
			throw std::invalid_argument("State not found");

		m_actions[state] = lambda;
	}
	
	void addTransition(const TState& startState, const TState& finalState, const std::function<void()>& lambda)
	{
		if (m_states.count(startState) == 0 || m_states.count(finalState) == 0)
			throw std::invalid_argument("State not found");

		m_transitions[{startState, finalState}] = lambda;
	}

	void transitionTo(const TState& state)
	{
		if (m_states.count(state) == 0)
			throw std::invalid_argument("State not found");

		if (m_currentState.has_value() && m_transitions.count({m_currentState.value(), state}) > 0)
			m_transitions[{m_currentState.value(), state}]();
		else if (m_currentState.has_value() && m_transitions.count({m_currentState.value(), state}) == 0)
			throw std::invalid_argument("Transition not defined");

		m_currentState = state;
	}

	void update() const
	{
		if (!m_currentState.has_value())
			throw std::logic_error("No state has been set");

		m_actions.at(m_currentState.value())();
	}


private:
	std::set<TState> m_states;
	std::optional<TState> m_currentState;
	std::map<TState, std::function<void()>> m_actions;
	std::map<std::pair<TState, TState>, std::function<void()>> m_transitions;
};

# endif // STATE_MACHINE_HPP