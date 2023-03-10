import numpy as np

class GridWorld:
    def __init__(self):
        self.rows = 4
        self.cols = 4
        self.start_state = (3, 0)
        self.goal_state = (0, 3)
        self.states = [(i, j) for i in range(self.rows) for j in range(self.cols)]
        self.actions = ["up", "down", "left", "right"]
        self.rewards = {(0, 3): 1}

    def get_reward(self, state):
        return self.rewards.get(state, 0)

    def is_terminal(self, state):
        return state == self.goal_state

class SarsaAgent:
    def __init__(self, env, alpha=0.1, epsilon=0.1, gamma=0.9):
        self.env = env
        self.alpha = alpha
        self.epsilon = epsilon
        self.gamma = gamma
        self.Q = {(state, action): 0 for state in env.states for action in env.actions}

    def choose_action(self, state):
        if np.random.uniform() < self.epsilon:
            action = np.random.choice(self.env.actions)
        else:
            q_values = [self.Q.get((state, action), 0) for action in self.env.actions]
            max_q = max(q_values)
            count = q_values.count(max_q)
            if count > 1:
                best_actions = [a for a in self.env.actions if self.Q.get((state, a), 0) == max_q]
                action = np.random.choice(best_actions)
            else:
                action = self.env.actions[np.argmax(q_values)]
        return action

    def train(self, episodes=100):
        for i in range(episodes):
            state = self.env.start_state
            action = self.choose_action(state)
            while not self.env.is_terminal(state):
                next_state = self.get_next_state(state, action)
                next_action = self.choose_action(next_state)
                reward = self.env.get_reward(next_state)
                self.update_q(state, action, reward, next_state, next_action)
                state = next_state
                action = next_action

    def get_next_state(self, state, action):
        i, j = state
        if action == "up":
            next_state = (max(i-1, 0), j)
        elif action == "down":
            next_state = (min(i+1, self.env.rows-1), j)
        elif action == "left":
            next_state = (i, max(j-1, 0))
        elif action == "right":
            next_state = (i, min(j+1, self.env.cols-1))
        else:
            raise ValueError("Invalid action")
        return next_state

    def update_q(self, state, action, reward, next_state, next_action):
        old_value = self.Q.get((state, action), 0)
        next_value = self.Q.get((next_state, next_action), 0)
        td_error = reward + self.gamma * next_value - old_value
        new_value = old_value + self.alpha * td_error
        self.Q[(state, action)] = new_value

    def run(self):
        state = self.env.start_state
        path = [state]
        while not self.env.is_terminal(state):
            action = self.choose_action(state)
            next_state = self.get_next_state(state, action)
            path.append(next_state)
            state = next_state
        path.append(self.env.goal_state)

        with open('output.txt', 'w') as f:
            for state in path:
                f.write(str(state) + '\n')
