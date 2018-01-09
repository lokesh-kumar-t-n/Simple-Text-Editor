#include <stdio.h>
#include <stdlib.h>

#define OP_INSERT_CHAR 		'I' //Insert
#define OP_PRESSED_ENTER 	'N' //Newline
#define OP_PRESSED_DELETE 	'D' //Delete
#define OP_PRESSED_LEFT 	'h' //Left arrow equivalent in vi editor
#define OP_PRESSED_DOWN 	'j' //Down arrow equivalent in vi editor
#define OP_PRESSED_UP 		'k' //Up arrow equivalent in vi editor
#define OP_PRESSED_RIGHT 	'l' //Right arrow equivalent in vi editor
#define OP_PRESSED_HOME 	'H' //Home
#define OP_PRESSED_END 		'E' //End
#define OP_PRINT_LINE 		'P' //Print the current line
#define OP_PRINT_TEXT 		'T' //print the whole Text
#define OP_QUIT 			'Q' //Quit the editor

//Forward declaration; see the definition of struct for more details.
struct text_node;

//A node in
struct line_node {
	char ch;
	struct line_node *next_char;
	struct line_node *prev_char;
	struct text_node *line_header;
};

struct text_node {
	struct line_node *line_begin;
	struct text_node *next_line;
	struct text_node *prev_line;
};

//Creates the empty text.
//Empty text has one empty line, which has a newline '\n' as end of line.
struct text_node *create_text(struct line_node **pcursor);

//Free the whole text
void free_text(struct text_node *text);

//Insert a char just before the cursor.
//Cursor always points to a line_node.
//A line has a newline char as the end of line.
struct line_node *op_insert_char(struct line_node *cursor, char ch);

//“Enter” (i.e. create) a new line and the cursor moves to the new line.
//If the cursor was at the end of line, a new empty line is created, otherwise
//the current line is split into a new line starting from the character at the cursor.
struct line_node *op_pressed_enter(struct line_node *cursor);

//“Delete” a character at the cursor, or the end of line if the cursor is
//at the end of a line. Deleting an end of line is essentially concatenating
//the next line with the current line.
//No operation if the cursor is at the end of line of the last line.
struct line_node *op_pressed_delete(struct line_node *cursor);

//Move the cursor “Left” by a character (or to the end of line of
//above line if the cursor is at the beginning of a line).
//No operation if the cursor is the beginning of the first line.
struct line_node *op_pressed_left(struct line_node *cursor);

//Move the cursor “Down” to the below line (beginning of the below line).
//No operation if the cursor is already at the last line.
struct line_node *op_pressed_down(struct line_node *cursor);

//Move the cursor “Up” to the above line (beginning of the above line).
//No operation if the cursor is already at the first line.
struct line_node *op_pressed_up(struct line_node *cursor);

//Move the cursor “Right” by a character (or to the beginning of below line
//if the cursor is at the end of a line).
//No operation if the cursor is the end of line of the last line.
struct line_node *op_pressed_right(struct line_node *cursor);

//Move the cursor to “Home”, that is, beginning of the current line.
//No operation if the cursor is at the beginning of a line.
struct line_node *op_pressed_home(struct line_node *cursor);

//Move the cursor to “End”, that is, end of line the current line.
//No operation if the cursor is at the end of a line.
struct line_node *op_pressed_end(struct line_node *cursor);

//“Print Line” prints the current line.
void op_print_line(struct line_node *cursor);

//“Print Text” prints the whole text.
void op_print_text(struct text_node *text);

int main() {
	struct line_node *cursor;
	struct text_node *text = create_text(&cursor);
	char op[4];

	do {
    op[2]=' ';
		scanf(" %[^\n]", op);
		switch(op[0]) {
	        case OP_INSERT_CHAR:
	            cursor = op_insert_char(cursor, op[2]);
	            break;
	        case OP_PRESSED_ENTER:
	            cursor = op_pressed_enter(cursor);
	            break;
	        case OP_PRESSED_DELETE:
	            cursor = op_pressed_delete(cursor);
	            break;
	        case OP_PRESSED_LEFT:
	            cursor = op_pressed_left(cursor);
	            break;
	        case OP_PRESSED_DOWN:
	            cursor = op_pressed_down(cursor);
	            break;
	        case OP_PRESSED_UP:
	            cursor = op_pressed_up(cursor);
	            break;
	        case OP_PRESSED_RIGHT:
	            cursor = op_pressed_right(cursor);
	            break;
	        case OP_PRESSED_HOME:
	            cursor = op_pressed_home(cursor);
	            break;
	        case OP_PRESSED_END:
	            cursor = op_pressed_end(cursor);
	            break;
	        case OP_PRINT_LINE:
	            op_print_line(cursor);
	            break;
	        case OP_PRINT_TEXT:
	            op_print_text(text);
	            break;
	    }
	} while(op[0] != OP_QUIT);

	free_text(text);
	return 0;
}

struct text_node *create_text(struct line_node **pcursor){
    struct text_node* tmp;
    tmp=(struct text_node*)malloc(sizeof(struct text_node));
    tmp->line_begin=(struct line_node*)malloc(sizeof(struct line_node));
    tmp->line_begin->ch='\n';//make the first line with a new line character
    *pcursor=tmp->line_begin;/*pcursor will be pointing to one of the first character in the text.
		But as it will turn out, it can be pointing to first character of any line as the operations are performed*/
    tmp->line_begin->next_char=NULL;
    tmp->line_begin->prev_char=NULL;
    tmp->line_begin->line_header=tmp;
    tmp->next_line=NULL;
    tmp->prev_line=NULL;
    return(tmp);
}
void free_text(struct text_node *text){
    struct line_node* p,*q;
    while(text!=NULL){
        p=text->line_begin;//p is pointing to the beginning of the line
        while(p!=NULL){
            q=p->next_char;/*q will be pointing to the next character in the line if it exists
						so that the link is not lost before freeing */
            free(p);
            p=q;//make p point to whatever q is pointing to
        }
        text=text->next_line;//move to the next line once the current line is freed
    }
}
struct line_node *op_insert_char(struct line_node *cursor, char ch){
    struct line_node* tmp;
		tmp=(struct line_node*)malloc(sizeof(struct line_node));//get memory space for a new character
    tmp->ch=ch;//assign the character
    tmp->next_char=cursor;//the next character will be whatever the cursor is pointing to
		/* remember that Initially it is pointing to a new line character */
    tmp->prev_char=cursor->prev_char;//its previous character will be the cursor's previous character
    if(cursor->prev_char!=NULL){//if cursor is not in the beginning of the line
        cursor->prev_char->next_char=tmp;//re-assign the previous character's next character
    }
    else{//if the cursor is currently at the beginning of the line
      cursor->line_header->line_begin=tmp;//then the inserted character becomes the starting character of the line
    }
    cursor->prev_char=tmp;//and the cursor's previous character will be whatever is being inserted now
    tmp->line_header=cursor->line_header;//assign the line_header of tmp to the cursor's line_header as they will be same
    return(cursor);
}
struct line_node *op_pressed_enter(struct line_node *cursor){
    struct text_node* tmp;
    struct line_node* p;
		tmp=(struct text_node*)malloc(sizeof(struct text_node));
		p=(struct line_node*)malloc(sizeof(struct line_node));
    p->ch='\n';//the character to be inserted is a new line
    if(cursor->next_char==NULL){//if the cursor is already at the end of a line
        p->next_char=NULL;//the next and the previous character will be NULL
        p->prev_char=NULL;
        p->line_header=tmp;//the text_node will be the new text_node created
        tmp->next_line=cursor->line_header->next_line;//the next line will be that of cursor's line_header's next line
        tmp->prev_line=cursor->line_header;//and the previous line will be cursor's line_header itself
        tmp->line_begin=p;//the first character is of course p
        if(cursor->line_header->next_line!=NULL){//if the cursor is not at the end of the text
        cursor->line_header->next_line->prev_line=tmp;//re-assign the next_line's previous line to the newly created one
        }
        cursor->line_header->next_line=tmp;//and the cursor's line_header's next line will be the newly created one
        cursor=p;//make cursor point to the newly created character
    }
    else{//if cursor is not at end of line
        if(cursor->prev_char!=NULL){//if cursor is not at the beginning of the line
            cursor->prev_char->next_char=p;//then the cursor's previous character's next character will be new line i.e p
            p->line_header=cursor->line_header;//the line header would remain the same
            p->next_char=NULL;//next character is null
            p->prev_char=cursor->prev_char;//and the previous character is whatever was the previous character of cursor
            cursor->prev_char=NULL;/*since cursor will be in the next line, it will become the first character of the next line
						and hence it's previous character will be NULL*/
            tmp->line_begin=cursor;
            tmp->next_line=cursor->line_header->next_line;//the new line's next line will be whatever cursor's line_header of next line
            tmp->prev_line=cursor->line_header;//it's previous line will be cursor's line_header itself
            if(cursor->line_header->next_line!=NULL){//if cursor is not in the last line
            cursor->line_header->next_line->prev_line=tmp;//re-assign the previousline link of nextLine of cursor's line_header
            }
            cursor->line_header->next_line=tmp;//cursor's line_header of next line will be the new line created
            struct line_node* p=cursor;
            while(p!=NULL){//re-assign the line_header of characters to the right of cursor
                p->line_header=tmp;
                p=p->next_char;
            }
        }
        else{//if the cursor is at the beginning of the line,then we need to create a empty line
            p->next_char=NULL;//both its previous and next character will be NULL as it's empty line
            p->prev_char=NULL;
            tmp->line_begin=p;
            tmp->next_line=cursor->line_header;//the new line's nextLine will be same as cursor's line_header
            tmp->prev_line=cursor->line_header->prev_line;//it's previous line will be whatever cursor's line_header's previous line
            if(cursor->line_header->prev_line!=NULL){//if the cursor is not at the last line
                cursor->line_header->prev_line->next_line=tmp;//re-assign the cursor's line_header's nextLine's previous line's link
            }
            cursor->line_header->prev_line=tmp;//and the cursor's line_header's previous line will be the new line created
        }

    }
    return(cursor);
}
struct line_node *op_pressed_delete(struct line_node *cursor){
    if(cursor->ch=='\n'){//if the cursor is at the end of a line
        struct text_node* tmp=cursor->line_header->next_line;
        if(tmp!=NULL){//if cursor is not at the last line
            struct line_node* p=tmp->line_begin;//p will be pointing to the first character in the next line
            struct line_node* q=NULL;
            if(cursor->prev_char!=NULL){//if the cursor is not in the beginning i.e if the line is not an empty line
                cursor->prev_char->next_char=p;//then the cursor's previousline's next character will be the nextLine's first character
                p->prev_char=cursor->prev_char;//and the nextLine's previous character will be cursor's previous character's.
				        cursor->next_char=p;
								//thought the cursor needs to be deleted, since p is the new cursor's point, i have kept it's address in next_char
                cursor->line_header->next_line=tmp->next_line;//and the cursor's line_header nextLine will be it's next to next line
                if(tmp->next_line!=NULL){//if next to next line of current is line is not NULL,
                    tmp->next_line->prev_line=cursor->line_header;//next to next line's previous line will be current line
                }
								free(tmp);
                while(p!=NULL){//re-assign the line_header of the nextline's characters
                    p->line_header=cursor->line_header;
                    p=p->next_char;
                }
                q=cursor->next_char;//q is pointing to the cursor
            }
            else{//if cursor is at the last line
                if(cursor->line_header->prev_line!=NULL){//if cursor is not at the first line.i.e the current line is an empty line
                    cursor->line_header->prev_line->next_line=tmp;//then previous line's next line will be next line
                }
                if(cursor->line_header->next_line!=NULL){//if cursor is not at the last line
                    cursor->line_header->next_line->prev_line=cursor->line_header->prev_line;//nextline's previous line will be previous line
                }
                q=cursor->line_header->next_line->line_begin;//q is pointing to the next line's first character
            }
            free(cursor);
            cursor=q;
        }
    }
    else{//if cursor is not at the end of the line
        if(cursor->prev_char!=NULL){//if cursor is not at the beginning of the line
            cursor->prev_char->next_char=cursor->next_char;//then, previous character's next character will be next character
				}
				else{//if cursor is at the beginning of the line
						cursor->line_header->line_begin=cursor->next_char;//then the starting character of the line will be next character
				}
        if(cursor->next_char!=NULL){//if cursor is not at the last character
        		cursor->next_char->prev_char=cursor->prev_char;//then, next character's previous character will be previous character
        }
        struct line_node* q=cursor->next_char;
        free(cursor);
        cursor=q;

    }
    return(cursor);
}
struct line_node *op_pressed_left(struct line_node *cursor){
    if(cursor->prev_char==NULL){//if the cursor is at the beginning of the line
        struct text_node* tmp=cursor->line_header->prev_line;
        if(tmp!=NULL){//if the cursor is not at the first line
            struct line_node* p=tmp->line_begin;
            struct line_node* q=NULL;
            while(p!=NULL){//move till the last character
                q=p;
                p=p->next_char;
            }
            cursor=q;//assign the cursor as the last character
        }
    }
    else{//if the cursor is not at the beginning of the line
        cursor=cursor->prev_char;//cursor will point to it's previous character
    }
    return(cursor);
}
struct line_node *op_pressed_down(struct line_node *cursor){
    struct text_node* p=cursor->line_header->next_line;
    if(p!=NULL){//if the cursor is not at the last line
        cursor=p->line_begin;//make the cursor point to the nextline's first character
    }
    return(cursor);
}
struct line_node *op_pressed_up(struct line_node *cursor){
    struct text_node* p=cursor->line_header->prev_line;
    if(p!=NULL){//if the cursor is not at the first line
        cursor=p->line_begin;//make the cursor point to the previous line's first character
    }
    return(cursor);
}
struct line_node *op_pressed_right(struct line_node *cursor){
    struct line_node* p=cursor->next_char;
    if(p==NULL){//if the cursor is at the end of line
        struct text_node* tmp=cursor->line_header->next_line;
        if(tmp!=NULL){//if the cursor is not at the last line
            cursor=tmp->line_begin;//make the cursor point to the nextline's first character
        }
    }
    else{//if the cursor is not at the end of line
        cursor=p;//make the cursor point to the next character.
    }
    return(cursor);
}
struct line_node *op_pressed_home(struct line_node *cursor){
    struct line_node* p=cursor->prev_char;
    struct line_node* q=NULL;
    while(p!=NULL){//move till the first character
        q=p;
        p=p->prev_char;
    }
    if(q!=NULL){//if it's not an empty line
        cursor=q;//make the cursor point to the first character
    }
    return(cursor);
}
struct line_node *op_pressed_end(struct line_node *cursor){
    struct line_node* p=cursor->next_char;
    struct line_node* q=NULL;
    while(p!=NULL){//move till the end of the line
        q=p;
        p=p->next_char;
    }
    if(q!=NULL){//if it's not an empty line
        cursor=q;//make the cursor point to the last character
    }
    return(cursor);
}
void op_print_line(struct line_node *cursor){
    if((cursor->prev_char==NULL)&&(cursor->ch=='\n')){/*if it's an empty line, then don't print it.
		However it might be absultely fine to print an empty line at times. So remove this condition if you want to print an empty line as well*/
        return;
    }
    struct line_node* p=cursor->line_header->line_begin;//move to the first character of the current line
    while(p!=NULL){//print all the characters in the line
        printf("%c",p->ch);
        p=p->next_char;
    }
}
void op_print_text(struct text_node *text){
    struct line_node* p=NULL;
    struct text_node* help=NULL;
    while(text!=NULL){//move to the first character
        help=text;
        text=text->prev_line;
    }
    text=help;
    while(text!=NULL){//print all the lines
        op_print_line(text->line_begin);
        text=text->next_line;
    }
}
