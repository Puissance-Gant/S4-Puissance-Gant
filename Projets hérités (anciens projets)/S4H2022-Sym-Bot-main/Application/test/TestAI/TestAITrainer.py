from test.TestAI.VectorTester import *
from AI.AITrainer import *


class TestAITrainer(VectorTester):

    def test_classification_vector(self):
        aitrainer = self.prepare_aitrainer()
        self.assertMatrixEqual(np.array([[1, 0, 0, 0, 0], [1, 0, 0, 0, 0], [1, 0, 0, 0, 0],
                                         [0, 1, 0, 0, 0], [0, 1, 0, 0, 0], [0, 1, 0, 0, 0],
                                         [0, 0, 1, 0, 0], [0, 0, 1, 0, 0], [0, 0, 1, 0, 0],
                                         [0, 0, 0, 1, 0], [0, 0, 0, 1, 0], [0, 0, 0, 1, 0],
                                         [0, 0, 0, 0, 1], [0, 0, 0, 0, 1], [0, 0, 0, 0, 1]]),
                               aitrainer.Y)

    def test_input_vector(self):
        aitrainer = self.prepare_aitrainer(False)
        input_exp = np.array([1, 0.5920, 0.2459, 1.2323, 0.0365, 0.3358, -9.4305, 0.1084])
        input2 = aitrainer.X[4, :]
        self.assertVectorEqual(input_exp, input2)

    def test_gradient_descent(self):
        aitrainer = self.prepare_aitrainer()
        aitrainer.grad_descent(np.zeros((8, 5)), 0.2, 0.01)
        test_vector = np.array([[0, 0, 0, 0, 0, -10, 0]])
        result = aitrainer.evaluate(test_vector)
        self.assertVectorEqual(result[0, :] > 0.5, np.array([1, 0, 0, 0, 0]))

    def prepare_aitrainer(self, scaling=True):
        aitrainer = AITrainer(7, 5)
        aitrainer.read_data('CSV_Data_Example.txt', scaling)
        return aitrainer


if __name__ == '__main__':
    unittest.main()
