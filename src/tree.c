#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"

tree_node* create_node(NODE_TYPE enum_type, int lineno,int childnum,...){
    tree_node* cur_node = (tree_node*)malloc(sizeof(tree_node));
    cur_node->node_type = enum_type;
    cur_node->line_no = lineno;
    cur_node->child_num = 0;
    cur_node->int_val = 0ll;
    cur_node->float_val = 0.0;
    if(childnum==0){
        return cur_node;
    }
    va_list arg_ptr;             //arg_ptr
    va_start(arg_ptr,childnum);   // point to arg after childnum
    for(int i = 0; i< childnum; ++i){
        cur_node->child_node[cur_node->child_num++] = va_arg(arg_ptr, tree_node*);
    }
    return cur_node;
}

tree_node* create_token_node(NODE_TYPE enum_type, int lineno,int val_selector,...){
    tree_node* cur_node = (tree_node*)malloc(sizeof(tree_node));  
    cur_node->node_type = enum_type;
    cur_node->line_no = lineno;
    cur_node->child_num = 0;
    va_list arg_ptr;
    switch(val_selector){
      case SELECTOR_INT:
        va_start(arg_ptr,val_selector);
        cur_node->int_val = va_arg(arg_ptr, long long int);
        cur_node->float_val = 0.0;
        break;
      case SELECTOR_FLOAT:
        va_start(arg_ptr,val_selector);
        cur_node->int_val = 0ll;
        cur_node->float_val = va_arg(arg_ptr, double);
        break;
      case SELECTOR_TYPE:
        va_start(arg_ptr,val_selector);
        strcpy(cur_node->node_name, va_arg(arg_ptr, char*));
        break;
      case SELECTOR_ID:
        va_start(arg_ptr,val_selector);
        strcpy(cur_node->node_name , va_arg(arg_ptr, char*));
        break;
      default:
        break;
    }
    return cur_node;
}

void traverse(tree_node* root,int cur_deep){
    // todo INT FLOAT TYPE ID
    for(int d = 0; d< cur_deep;++d){
        fprintf(stderr, "  ");
    }
    switch(root->node_type){
      case ENUM_INT:
        fprintf(stderr, "%s: %lld\n", type_name[root->node_type], root->int_val);
        break;
      case ENUM_FLOAT:
        fprintf(stderr, "%s: %lf\n", type_name[root->node_type], root->float_val);
        break;
      case ENUM_ID:
        fprintf(stderr, "%s: %s\n", type_name[root->node_type], root->node_name);
        break;
      case ENUM_TYPE:
        fprintf(stderr, "%s: %s\n", type_name[root->node_type], root->node_name);
        break;
      default:
        if(root->node_type<ENUM_Epsilon){
          fprintf(stderr, "%s\n", type_name[root->node_type]);
        }else{
          fprintf(stderr, "%s (%d)\n", type_name[root->node_type], root->line_no);
        }
        break;
    }
    for(int i = 0; i < root->child_num; ++i){
        if(root->child_node[i]!=NULL){
            traverse(root->child_node[i],cur_deep+1);
        }
    }
}