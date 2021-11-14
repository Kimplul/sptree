# sptree
A spin on self-balancing binary search trees.

# Theory
AVL-trees promise close to optimal tree layout for lookup, but use a lot of memory and require slow
(comparatively speaking) balancing operations. Red-black trees offer quicker manipulation with slightly
less optimal tree layout, and splay trees offer blazing fast insertion/deletion by balancing trees
during lookup with a 'close enough' approach.

This repository demonstrates my attempt to combine aspects of these trees.

# Properties
`sp-trees` as I've implemented them here, use `hint`s to decide whether to do a balancing operation or not.
They are similar to AVL-trees' `height` property, but aren't required to be absolutely accurate.
A `hint` gives an approximation of the longest chain of nodes under the node the `hint` is attached to.

This implementation also uses `parent` pointers, like in red-black trees, but you could in theory just use a
recursive function that rebalances the tree. This would however have a time penalty.

# Operations
`sp-trees` use four typical `BST`-operations, `rotate_left`, `rotate_right` and `replace_right`, `replace_left`.
`replace_right` and `replace_left` are only used during the removal of a node, in typical `BST` fashion: Get the
next/last node in the right/left subtree and replace the node to be deleted with the found node.
`rotate_left` and `rotate_right` are used in a separate `update` phase, during which the tree is rebalanced.
This phase follows both the `insert` and `remove` phases in this implementation, but in theory you could have any
combination of `insert/remove/lookup -> update` sequences. The more times an `update` is performed, the closer to 
optimal the tree layout becomes, but each `update` of course has some time penalty.

The `update` function is relatively simple: If a given node leans to the right or left over a treshold, the node
is rotated left or right. The node's `hint` is always updated, and if the node's `hint`
is zero or different from before the update, the node's parent (before the operations) is modified.

Insertion is straight-forward, works just like any other `BST` and then you just call `update` on the inserted node.

Deletion is a bit more complex, but the idea is similar to other `BST`s: If the node to be removed has a right node,
replace the node to be deleted with the `first` node in the right subtree and call `update` on the right node of the
replacement node. Similarly for the left node if the node to be deleted doesn't have a right node. If the node doesn't
have any children (left or right), it can be directly removed and `update` can be called on the parent of the node.

# Performance
Full transparency, I don't have any mathematical proof that this method promises "39.8 % worse than optimal tree layouts" or
anything substantial like that, but I did compare this code against these two implementations of other binary trees:

+ https://github.com/louiswins/RB-Tree
+ https://github.com/iglenwoo/avl_tree

Not claiming that they are in any way state-of-the-art, but I chose them because when I was testing my code I used a fixed
`sp_node` with integers as the data payload, which these two also use. All in all, my code was about ~20 % faster with
insertion and deletion, whereas lookup was only about ~10 % slower. I tested the trees by
inserting a large number of random numbers, a large number of sequential numbers and then a large number of reverse sequential
numbers (10, 9, 8...). The 'large number' ranged from `1 000 000` to `1 000 000 000`.
This would maybe point towards this type of tree being useful in situations where insertion and
deletion is the key bottleneck, and each element in the tree is only accessed a few times before being removed or left alone.

The maximum tree depth for the case where `1 000 000 000` integers were inserted randomly was 40 for my trees, and 29 for the two other types.
This shows that my trees indeed do produce less optimal tree layouts on average, and the theoretical worst lookup time in this case was ~30 % slower,
but on average the lookup time was be a lot closer to equal as some nodes are way under 29 and most are fairly close to 29.

# Implementing other data types
Implementing other data types closely follows the method used by the Linux kernel, namely that you should embed a `struct sp_node`
into your data node, and implement the operations that require comparisons yourself, so any of
+ Insert
+ Remove
+ Find

To actually modify the tree, call `sp_insert` and `sp_remove` with the necessary arguments. Look into `test.c` for an example that uses integers
as the data type.
