NAME		=	libftpp.a

SRCS		=	

OBJS		=	$(SRCS:.cpp=.o)

BSRCS		=		

BOBJS		=	$(BSRCS:.cpp=.o)

CPPC		=	c++

CPPFLAGS	=	-Wall -Wextra -Werror -std=c++11

AR			=	ar rc

RAN			=	ranlib

RM			=	rm -f

.cpp.o:
			$(CPPC) $(CPPFLAGS) -c $< -o $(<:.cpp=.o)

$(NAME):	$(OBJS)
			$(AR) $(NAME) $(OBJS)
			$(RAN) $(NAME)

bonus:		$(NAME) $(BOBJS)
			$(AR) $(NAME) $(BOBJS)
			$(RAN) $(NAME)

clean:
			$(RM) $(OBJS) $(BOBJS)

fclean:		clean
			$(RM) $(NAME)

all:		$(NAME)

re:			fclean all
