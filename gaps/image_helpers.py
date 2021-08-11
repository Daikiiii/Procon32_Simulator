import numpy as np
from gaps.piece import Piece


def flatten_image(image, columns,rows, indexed=False):
    """
    画像を断片画像のリストに変換する関数
    入力画像は正方形に分割され、フラットなリストに変換される。
    各リストの要素は PIECE_SIZE x PIECE_SIZE x 3
    戻り値は　平滑化されたリスト,断片画像のサイズ

    :params image:      Input image.
    :params columns 列
    :params rows    行
    保留
    :params indexed:    If True list of Pieces with IDs will be returned, otherwise just plain list of ndarray pieces

    使用法::

        >>> from gaps.image_helpers import flatten_image
        >>> flat_image = flatten_image(image, rows,columns)

    """
    #piece_size 取得 本番では正方形だから
    piece_size=image.shape[0]//rows

    pieces = []

    # Crop pieces from original image
    for y in range(rows):
        for x in range(columns):
            left, top, w, h = x * piece_size, y * piece_size, (x + 1) * piece_size, (y + 1) * piece_size
            piece = np.empty((piece_size, piece_size, image.shape[2]))
            piece[:piece_size, :piece_size, :] = image[top:h, left:w, :]
            pieces.append(piece)

    if indexed:
        pieces = [Piece(value, index) for index, value in enumerate(pieces)]

    return pieces, piece_size
    
def assemble_image(pieces, rows, columns):
    """Assembles image from pieces.

    Given an array of pieces and desired image dimensions, function
    assembles image by stacking pieces.

    :params pieces:  Image pieces as an array.
    :params rows:    Number of rows in resulting image.
    :params columns: Number of columns in resulting image.

    使用法::

        >>> from gaps.image_helpers import assemble_image
        >>> from gaps.image_helpers import flatten_image
        >>> pieces, rows, cols = flatten_image(...)
        >>> original_img = assemble_image(pieces, rows, cols)

    """
    vertical_stack = []
    for i in range(rows):
        horizontal_stack = []
        for j in range(columns):
            horizontal_stack.append(pieces[i * columns + j])
        vertical_stack.append(np.hstack(horizontal_stack))
    return np.vstack(vertical_stack).astype(np.uint8)
