#include "push_swap.h"
#include "unit.h"

int	test_normalize()
{
	{
		t_stack *stack = NULL;

		stack_push(&stack, 5);
		stack_push(&stack, 12);
		stack_push(&stack, 7);
		stack_push(&stack, 1);

		stack_normalize(stack);

		int pop1 = stack_pop_normalized(&stack);
		int pop2 = stack_pop_normalized(&stack);
		int pop3 = stack_pop_normalized(&stack);
		int pop4 = stack_pop_normalized(&stack);

		int is_correct = pop1 == 0 && pop2 == 2 && pop3 == 3 && pop4 == 1;

		TEST (is_correct, 1, "Normalize function dind't work for all positive numbers");
	}

{
		t_stack *stack = NULL;

		stack_push(&stack, 5);	// 3
		stack_push(&stack, 12);	// 4
		stack_push(&stack, 0);	// 1
		stack_push(&stack, 1);	// 2
		stack_push(&stack, -1); // 0

		stack_normalize(stack);

		int pop1 = stack_pop_normalized(&stack);
		int pop2 = stack_pop_normalized(&stack);
		int pop3 = stack_pop_normalized(&stack);
		int pop4 = stack_pop_normalized(&stack);
		int pop5 = stack_pop_normalized(&stack);

		int is_correct = pop1 == 0 && pop2 == 2 && pop3 == 1 && pop4 == 4 && pop5 == 3;

		TEST (is_correct, 1, "Normalize function dind't work with negative value");
	}

	return (1);
}

REGISTER_TEST(test_normalize);
