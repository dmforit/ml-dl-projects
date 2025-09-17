import numpy as np
from nearest_neighbors import KNNClassifier


def get_accuracy(predict, target):
    """
    Estimates the fraction of predictions the model got right.
    :param predict: np.ndarray. Our predictions
    :param target: np,ndarray. Our true values
    :return: float. The result accuracy
    """
    return np.count_nonzero(predict == target) / target.shape[0]


def cross_predict_score(values, weights, target, function, k_list, k_dict, n):
    """
    Estimates the score of the prediction specified in function
    :param values: the matrix of labels
    :param weights: the matrix of weights
    :param target: real values
    :param function: estimator
    :param k_list: cross-validation k_list
    :param k_dict: the dict for result estimations
    :param n: current fold
    :return: prediction score
    """
    if not len(k_list):
        return

    prev_k = 0
    unique = np.unique(values)
    weights_dict = {un: 0 for un in unique}
    for k in k_list:
        weights_dict = {un: weights_dict[un] + np.sum(weights[:, prev_k:k] * (values[:, prev_k:k] == un), axis=1)
                        for un in unique}
        max_array = np.array([weights_dict[un] for un in unique])
        predict = unique[np.argmax(max_array, axis=0)]
        k_dict[k][n] = function(predict, target)
        prev_k = k


def kfold(n, n_folds):
    """
    Creates indices for train and test datasets
    :param n: A dataset size
    :param n_folds: The count of the folds
    :return: list of tuples with train and test indices
    """

    indices = np.arange(n)
    result = list()

    if n % n_folds:
        first_ind = np.arange((n % n_folds) * (n // n_folds + 1))
        first_tests = first_ind.reshape(n % n_folds, -1)
        result = [(np.delete(indices, test), test) for test in first_tests]
        second_ind = np.arange((n % n_folds) * (n // n_folds + 1), n)
    else:
        second_ind = indices

    second_tests = second_ind.reshape(n_folds - n % n_folds, -1)
    for test in second_tests:
        result.append((np.delete(indices, test), test))
    return result


def knn_cross_val_score(x, y, k_list, score, cv, **kwargs):
    """
    Counts cross validation score for each k in k_list
    :param x: dataset objects
    :param y: target values
    :param k_list: hyperparameters for knn
    :param score: the name of the estimation metric
    :param cv: list of tuples containing indices for train and test splitting
    :param kwargs: arguments for KNNClassifier
    :return: dict of scores for each k
    """
    folds_count = 3
    eps = 1e-5
    score_functions = [get_accuracy]
    score_name = {'accuracy': 0}
    score_fun = score_functions[score_name[score]]

    knn = KNNClassifier(k_list[-1], **kwargs)
    if cv is None:
        cv = kfold(x.shape[0], folds_count)

    k_dict = dict()
    for k in k_list:
        k_dict[k] = np.zeros(len(cv))

    for n, (train, test) in enumerate(cv):
        knn.fit(x[train], y[train])
        if knn.weights:
            dist, ind = knn.find_kneighbors(x[test], knn.weights)
            weight_matrix = 1.0 / (dist + eps)
        else:
            dist, ind = None, knn.find_kneighbors(x[test], knn.weights)
            weight_matrix = np.ones_like(ind)
        values = y[train][ind]
        cross_predict_score(values, weight_matrix, y[test], score_fun, k_list, k_dict, n)

    return k_dict
