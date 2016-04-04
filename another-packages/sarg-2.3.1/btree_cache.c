/*
 * SARG Squid Analysis Report Generator      http://sarg.sourceforge.net
 *                                                            1998, 2010
 *
 * SARG donations:
 *      please look at http://sarg.sourceforge.net/donations.php
 * Support:
 *     http://sourceforge.net/projects/sarg/forums/forum/363374
 * ---------------------------------------------------------------------
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.
 *
 */

#include "include/conf.h"
#include "include/defs.h"

#define AVL_SINGLE_RIGHT_ROTATION 1
#define AVL_SINGLE_LEFT_ROTATION  2
#define AVL_DOUBLE_RIGHT_ROTATION 3
#define AVL_DOUBLE_LEFT_ROTATION  4

struct bt {
           char value[64], targetattr[256];
           struct bt *left, *right;
           int balanceinfo;
};

struct bt *root_bt = NULL;
int sizeof_bt;

FILE *dbgfp = NULL;

void set_balance_info(struct bt *node);
struct bt *get_disbalanced_node(struct bt *node);
void balance_node(struct bt *node);


struct bt *insert_node(struct bt *root, const char *item, const char *value)
{
	struct bt *new_item_bt = NULL;
	if (!root)
	{
		new_item_bt = malloc(sizeof_bt);
		new_item_bt->left = NULL;
		new_item_bt->right = NULL;
		strncpy(new_item_bt->value, item, 64);
		strncpy(new_item_bt->targetattr, value, 256);
		new_item_bt->balanceinfo = 0;
		return new_item_bt;
	}
	else
	{
		int result = strncmp(root->value, item, 64);
		if ( result > 0 )
		{
			new_item_bt = insert_node(root->left, item, value);
			if (!root->left)
				root->left = new_item_bt;
		}
		else
			if (result < 0)
			{
				new_item_bt = insert_node(root->right, item, value);
				if (!root->right)
					root->right = new_item_bt;
			}
			else
				return NULL;

	}
	return new_item_bt;
}




void balance_tree(struct bt *node)
{
	struct bt *disbalanced_node = NULL;
	do
	{
		set_balance_info(root_bt);
		disbalanced_node = get_disbalanced_node(root_bt);
		if (disbalanced_node)
			balance_node(disbalanced_node);
	}
	while (disbalanced_node);
	set_balance_info(root_bt);
}



void delete_tree(struct bt *root)
{
	if (root)
	{
		delete_tree(root->left);
		delete_tree(root->right);
		free(root);
		root = NULL;
	}
}

struct bt *search_item(struct bt *root, const char *item)
{
	int result;
	while (root && (result = strncmp(root->value, item, 64)))
	{
		if (result > 0)
			root = root->left;
		else
			root = root->right;
	}
	return root;
}

int get_length(struct bt *node, int d)
{
	int l_depth = d, r_depth = d;
	if (node->left)
		l_depth = get_length(node->left, d+1);
	if (node->right)
		r_depth = get_length(node->right, d+1);
	return( ( l_depth > r_depth ) ? l_depth : r_depth );
}

struct bt *get_parent(struct bt *node)
{
	if (node == root_bt)
		return NULL;
	else
	{
		int result;
		struct bt *prev = root_bt, *tmp = root_bt;
		while (tmp && (result = strncmp(node->value, tmp->value,64)))
		{
			if (result < 0)
			{
				prev = tmp;
				tmp = tmp->left;
			}
			else
			{
				prev = tmp;
				tmp = tmp->right;
			}
		}
		if (tmp)
			return prev;
		else
			return NULL;
	}
}

void set_balance_info(struct bt *node)
{
	int l_depth = 0, r_depth = 0;
	if (node->left)
	{
		l_depth = get_length(node->left, 1);
		set_balance_info(node->left);
	}
	if (node->right)
	{
		r_depth = get_length(node->right, 1);
		set_balance_info(node->right);
	}
	node->balanceinfo = r_depth - l_depth;
}

void rotate_right(struct bt *node)
{
	struct bt *left, *right, *tmp, *parent = get_parent(node);
	left = node->left;
	right = node->left->right;
	tmp = node;
	node = left;
	tmp->left = right;
	node->right = tmp;

	if (root_bt == tmp)
		root_bt = node;
	else
	{
		if (parent->left == tmp)
			parent->left = node;
		else
			if (parent->right == tmp)
				parent->right = node;
	}
}

void rotate_left(struct bt *node)
{
	struct bt *left, *right, *tmp, *parent = get_parent(node);
	left = node->right->left;
	right = node->right;
	tmp = node;
	node = right;
	tmp->right = left;
	node->left = tmp;

	if (root_bt == tmp)
		root_bt = node;
	else
	{
		if (parent->left == tmp)
			parent->left = node;
		else
			if (parent->right == tmp)
				parent->right = node;
	}

}

int get_disbalance_type(struct bt *node)
{
	if (node->balanceinfo < 0)
		if (node->left->balanceinfo > 0)
			return AVL_DOUBLE_RIGHT_ROTATION;
		else
			return AVL_SINGLE_RIGHT_ROTATION;
	else
		if (node->right->balanceinfo < 0)
			return AVL_DOUBLE_LEFT_ROTATION;
		else
			return AVL_SINGLE_LEFT_ROTATION;
}

void balance_node(struct bt *node)
{
	switch (get_disbalance_type(node))
	{
		case AVL_SINGLE_RIGHT_ROTATION:
			rotate_right(node);
			break;
		case AVL_SINGLE_LEFT_ROTATION:
			rotate_left(node);
			break;
		case AVL_DOUBLE_RIGHT_ROTATION:
			rotate_right(node);
			rotate_right(node);
			break;
		case AVL_DOUBLE_LEFT_ROTATION:
			rotate_left(node);
			rotate_left(node);
			break;
		default:
   exit(EXIT_FAILURE);
			break;

	}
}

struct bt *get_disbalanced_node(struct bt *node)
{
	struct bt *rdn;
	if (fabs(node->balanceinfo) > 1)
		return node;
	else
		if (node->left)
		{
			rdn = get_disbalanced_node(node->left);
			if (rdn)
				return rdn;
		}
		if (node->right)
		{
			rdn = get_disbalanced_node(node->right);
			if (rdn)
				return rdn;
		}
		return NULL;
}

void init_cache(void)
{
	root_bt = NULL;
	sizeof_bt = sizeof(struct bt);
}


int insert_to_cache(const char *key, const char *value)
{

	struct bt *root = NULL;

	char  strict_chars[] = " ~!@^&(){}|<>?:;\"\'\\[]`,\r\n\0", *strict_chars_ptr;

	strict_chars_ptr = strict_chars;
	while (*strict_chars_ptr)
	{
		char *strict_chr_ptr = strchr(key, *strict_chars_ptr);
		if (strict_chr_ptr)
			*strict_chr_ptr = '\0';
		strict_chars_ptr++;
	}
	if ((root = (insert_node(root_bt, key, value))))
	{
		if (!root_bt)
			root_bt = root;
		balance_tree(root_bt);
		return 0;
	}
	else
		return 1;

}

char *search_in_cache(const char *key)
{
	struct bt *node;
	if ((node = search_item(root_bt, key)))
	{
		return node->targetattr;
	}
	else
		return NULL;
}

void destroy_cache(void)
{
	delete_tree(root_bt);
	root_bt = NULL;
}
