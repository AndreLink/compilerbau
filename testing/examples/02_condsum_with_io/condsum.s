.text

.data
					global: .space 4

.text

.data
					a: .space 40

.text

function_func:
					move $10, $4
					move $11, $5
					move $12, $6
					li   $15, 0
					move $13, $15
					li   $16, 0
					move $14, $16
while_label_0:
					xor  $17, $17, $17
					slt  $17, $13, $11

					beqz $17, label_1
					move $8, $10
					move $9, $13
					mul  $9, $9, 4
					add  $8, $8, $9
					sw   $18, 0($8) 
					xor  $19, $19, $19
					sle  $19, $18, $12

					beqz $19, label_3
					li   $20, 1
					xor  $21, $21, $21
					add  $21, $14, $20
					move $14, $21
					j label_4
label_3:
label_4:
					li   $22, 1
					xor  $23, $23, $23
					add  $23, $13, $22
					move $13, $23
					addiu $sp, $sp, -4
					sw   $13, 0($sp)
					move $4, $13
					jal  function_print
					addiu $sp, $sp, 4
					j while_label_0
label_1:
					move $2, $14
					jr   $ra

function_main:
					la   $8, global
					lw   $10, 0($8) 
					li   $11, 1
					la   $8, global
					sw   $11, 0($8)
					la   $8, a
					lw   $12, 0($8) 
					li   $13, 5
					li   $14, 5
					xor  $15, $15, $15
					add  $15, $13, $14
					li   $16, 5
					addiu $sp, $sp, -12
					la   $8, a
					sw   $8, 0($sp)
					move $4, $8
					sw   $15, 4($sp)
					move $5, $15
					sw   $16, 8($sp)
					move $6, $16
					jal  function_func
					addiu $sp, $sp, 12
					move $17, $2
					move $2, $17
					addiu $sp, $sp, -0
					jal  function_scan
					addiu $sp, $sp, 0
					move $18, $2
					addiu $sp, $sp, -4
					sw   $18, 0($sp)
					move $4, $18
					jal  function_print
					addiu $sp, $sp, 4
					li   $20, 0
					addiu $sp, $sp, -4
					sw   $20, 0($sp)
					move $4, $20
					jal  function_exit
					addiu $sp, $sp, 4
					jr   $ra
