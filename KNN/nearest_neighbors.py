import numpy as np
from sklearn.neighbors import NearestNeighbors
from distances import euclidean_distance, cosine_distance


class KNNClassifier:
    def __init__(self, k, strategy, metric, weights, test_block_size):
        """
        Initialization for KNNClassifier
        :param k: int. The count of the nearest neighbors - hyperparameter
        :param strategy: str. 'my_own', 'brute', 'kd_tree', 'ball_tree'
        :param metric: str. 'euclidean', 'cosine'
        :param weights: bool. True for the weighted KNN method and False otherwise
        :param test_block_size: The size of test data blocks
        """
        self.__is_valid(k, strategy, metric, weights, test_block_size)  # checking arguments
        self.k = k
        self._eps = 1e-5
        self.strategy = strategy
        self.metric = metric
        self.weights = weights
        self.test_block_size = test_block_size
        self._implemented_data = None
        self._fitted_data = None
        self._targets_values = None

    @staticmethod
    def __is_valid(k, strategy, metric, weights, test_block_size):
        """
        Checking if the arguments are acceptable
        :param k: int. The count of the nearest neighbors - hyperparameter
        :param strategy: str. 'my_own', 'brute', 'kd_tree', 'ball_tree'
        :param metric: str. 'euclidean', 'cosine'
        :param weights: bool. True for the weighted KNN method and False otherwise
        :param test_block_size: The size of test data blocks
        :return: Nothing to return
        """
        strats = ['my_own', 'brute', 'kd_tree', 'ball_tree']
        metrics = ['euclidean', 'cosine']

        assert isinstance(k, int), 'k should be an int'
        assert strategy in strats, 'Use only acceptable strategies'
        assert metric in metrics, 'Use only acceptable metrics'
        assert isinstance(weights, bool), 'weights should be a bool'
        assert isinstance(test_block_size, int), 'test_block_size should be an int'

        if strategy in ['kd_tree', 'ball_tree'] and metric != 'euclidean':
            raise AssertionError('Use euclidean metrics with kd_tree and ball_tree')

    def _kneighbors(self, x, return_distance=True):
        """
        Finds k nearest neighbors for each object in the dataset but with a choice of algorithm.
        :param x: np.ndarray. Dataset
        :return: Tuple. The first element is a matrix[x.shape[0], k] of distances where [i, j] element is the
        distance between the i-th object of x and its j-th neighbor. The second element is a
        matrix[X.shape[0], k] of indices where [i, j] element is the index of the j-th nearest neighbor from
        the dataset for the i-th object. If return_distance is False then the function returns an empty np.ndarray as
        the first element of the tuple.
        """
        dist = np.array([])

        if self._implemented_data is not None:
            if return_distance:
                dist, ind = self._implemented_data.kneighbors(x, self.k)
            else:
                ind = self._implemented_data.kneighbors(x, self.k, False)
        else:
            if self.metric == 'euclidean':
                dist_matrix = euclidean_distance(x, self._fitted_data)
            else:
                dist_matrix = cosine_distance(x, self._fitted_data)

            index_array = np.argpartition(dist_matrix, kth=self.k - 1, axis=-1)[:, :self.k]
            rows = np.arange(index_array.shape[0])[:, None]
            dist_array = dist_matrix[rows, index_array]
            sorted_index = np.argsort(dist_array, axis=-1)
            dist = dist_array[rows, sorted_index]
            ind = index_array[rows, sorted_index]

        return dist, ind

    def fit(self, x, y):
        """
        Fitting our model to the train dataset
        Saves data under the method requirements
        :param x: np.ndarray. Train dataset
        :param y: np.ndarray. Target values
        :return: self
        """
        self._targets_values = y

        if self.strategy != 'my_own':
            self._implemented_data = NearestNeighbors(n_neighbors=self.k, algorithm=self.strategy)
            self._implemented_data.fit(x)
        else:
            self._fitted_data = x
        return self

    def find_kneighbors(self, x, return_distance):
        """
        Finds k nearest neighbors for each object in the dataset
        :param x: np.ndarray. Dataset
        :param return_distance: bool. Returns the first ndarray only if True
        :return: Tuple. The first element is a matrix[x.shape[0], k] of distances where [i, j] element is the
        distance between the i-th object of x and its j-th neighbor. The second element is a
        matrix[X.shape[0], k] of indices where [i, j] element is the
        index of the j-th nearest neighbor from the dataset for the i-th object.
        """
        distances = np.array([])
        indices = np.array([])

        for i in range(0, x.shape[0], self.test_block_size):
            end_index = i + self.test_block_size
            if end_index >= x.shape[0]:
                end_index = x.shape[0]

            dist, ind = self._kneighbors(x[i:end_index], return_distance)
            distances = np.concatenate([distances, dist]) if distances.size else dist
            indices = np.concatenate([indices, ind]) if indices.size else ind

        if return_distance:
            return distances, indices
        else:
            return indices

    def predict(self, x, dist=None, ind=None):
        """
        Returns the predicted classes' labels
        :param x: np.ndarray. Test dataset
        :param dist: np.ndarray. A matrix of distances
        :param ind: np.ndarray. A matrix of indices
        :return: np.ndarray. It's a matrix[X.shape[0]] of the classes' labels
        """
        if self.weights:
            distances, indices = self.find_kneighbors(x, True) \
                if dist is None or ind is None else (dist, ind)
            weight_matrix = 1.0 / (distances + self._eps)
        else:
            indices = self.find_kneighbors(x, False) if ind is None else ind
            weight_matrix = np.ones_like(indices)

        values = self._targets_values[indices]

        unique = np.unique(values)
        weights_dict = {un: np.sum(weight_matrix * (values == un), axis=1) for un in unique}
        max_array = np.array([weights_dict[un] for un in unique])
        target = unique[np.argmax(max_array, axis=0)]

        return target
