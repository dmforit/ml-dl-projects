import numpy as np


def euclidean_distance(x, y):
    """
    Returns matrix where the [i, j]-th element is the euclidean distance
    between the i-th object of x and the j-th object of y. x and y shouldn't contain zero
    :param x: np.ndarray with the shape of N x D
    :param y: np.ndarray with the shape of M x D
    :return: np.ndarray with the shape of N x M
    """
    x_squares = (x * x).sum(axis=1).reshape(-1, 1)
    y_squares = (y * y).sum(axis=1).reshape(-1, 1)
    scalar_product = x @ y.T

    x_squares_add1 = np.concatenate((x_squares, np.ones_like(x_squares)), 1)
    y_squares_add1 = np.concatenate((np.ones_like(y_squares), y_squares), 1)
    squares_sum = x_squares_add1 @ y_squares_add1.T

    distance = squares_sum - scalar_product * 2.0
    return np.sqrt(distance)


def cosine_distance(x, y):
    """
    Returns matrix where the [i, j]-th element is the cosine distance
    between the i-th object of x and the j-th object of y. x and y shouldn't contain zero
    :param x: np.ndarray with the shape of N x D
    :param y: np.ndarray with the shape of M x D
    :return: np.ndarray with the shape of N x M
    """
    x_norms = np.sqrt((x * x).sum(axis=1).reshape(-1, 1))
    y_norms = np.sqrt((y * y).sum(axis=1).reshape(-1, 1))
    scalar_product = x @ y.T

    x_norms_add0 = np.concatenate((x_norms, np.zeros_like(x_norms)), 1)
    y_norms_add0 = np.concatenate((y_norms, np.zeros_like(y_norms)), 1)
    norms_product = x_norms_add0 @ y_norms_add0.T

    distance = 1.0 - scalar_product / norms_product
    distance = distance * (np.abs(distance) > 1e-8)
    return distance
