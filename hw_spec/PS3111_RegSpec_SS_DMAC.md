# Doorbell Register Description (QInfo)

## Circular Queue 0 (DMAC Submission Queue)

### Queue Base Address Register (Offset 03h\~00h)

Abbreviation: **QBASE_ADR**

Register Index: 03h\~00h

Default Value: 00h

Attribute: RW

+------+-----------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                | Default | Attributes |
+:=====+:============================================================================+:========+:===========+
| 31:0 | **QBASE_ADR :**                                                             | 00h     | RW         |
|      |                                                                             |         |            |
|      | 1\. QBASE_ADR must be 8 byte aligned address.                               |         |            |
|      |                                                                             |         |            |
|      | 2\. While VQ = 0, setting QBASE_ADR will result in QBASE_ADR = WPTR = RPTR. |         |            |
|      |                                                                             |         |            |
|      | 3\. While VQ = 1, QBASE_ADR is locked and can't be modified.                |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

### Write Pointer Register (Offset 07h\~04h)

Abbreviation: **WPTR**

Register Index: 07h\~04h

Default Value: 00h

Attribute: RO

  ---------------------------------------------------------------------------------
  Bit    Descriptions                                        Default   Attributes
  ------ --------------------------------------------------- --------- ------------
  31:0   Write pointer of circular queue.                    00h       RO

  ---------------------------------------------------------------------------------

### Read Pointer Register (Offset 0Bh\~08h)

Abbreviation: **RPTR**

Register Index: 0Bh\~08h

Default Value: 00h

Attribute: RO

  ---------------------------------------------------------------------------------
  Bit    Descriptions                                        Default   Attributes
  ------ --------------------------------------------------- --------- ------------
  31:0   Read pointer of circular queue.                     00h       RO

  ---------------------------------------------------------------------------------

### Write Pointer Increase Unit Register (Offset 0Dh\~0Ch)

Abbreviation: **WPIU**

Register Index: 0Dh\~0Ch

Default Value: 00h

Attribute: RW

+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                                              | Default | Attributes |
+:=====+:==========================================================================================================+:========+:===========+
| 15:9 | Reserve                                                                                                   | 00h     | RO         |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| 8:0  | **WPIU: (**FW triggers this field to maintain WPTR**)**                                                   | 00h     | RW         |
|      |                                                                                                           |         |            |
|      | 2\. Range of WPIU : 1 \~ (BUF_LEN-1)                                                                      |         |            |
|      |                                                                                                           |         |            |
|      | 3\. Only When VQ = 1 and low byte of WPIU is written, WPTR is incremented depending on the following rule |         |            |
|      |                                                                                                           |         |            |
|      | a\. WPTR = WPTR + (DATA_SIZE \* WPIU)                                                                     |         |            |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+

### Writable Count Register (Offset 0Fh\~0Eh)

Abbreviation: **WR_CNT**

Register Index: 0Fh \~ 0Eh

Default Value: 00h

Attribute: RO

+------+-----------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                | Default | Attributes |
+:=====+:============================================================================+:========+:===========+
| 15:9 | Reserve                                                                     | 00h     | RO         |
+------+-----------------------------------------------------------------------------+---------+------------+
| 8:0  | **WR_CNT:**                                                                 | 00h     | RO         |
|      |                                                                             |         |            |
|      | Indicate that how many amount of data can be written to the circular queue. |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

### Read Pointer Increase Unit Register (Offset 11h\~10h)

Abbreviation: **RPIU**

Register Index: 11h\~10h

Default Value: 00h

Attribute: RW

+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                                              | Default | Attributes |
+:=====+:==========================================================================================================+:========+:===========+
| 15:9 | Reserve                                                                                                   | 00h     | RO         |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| 8:0  | **RPIU: (**DMAC triggers this field to maintain RPTR**)**                                                 | 00h     | RW         |
|      |                                                                                                           |         |            |
|      | 1\. Range of RPIU : 1 \~ (BUF_LEN-1)                                                                      |         |            |
|      |                                                                                                           |         |            |
|      | 2\. Only When VQ = 1 and low byte of RPIU is written, RPTR is incremented depending on the following rule |         |            |
|      |                                                                                                           |         |            |
|      | a\. RPTR = RPTR + (DATA_SIZE \* RPIU)                                                                     |         |            |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+

### Readable Count Register (Offset 13h\~12h)

Abbreviation: **RD_CNT**

Register Index: 13h\~12h

Default Value: 00h

Attribute: RO

+------+----------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                               | Default | Attributes |
+:=====+:===========================================================================+:========+:===========+
| 15:9 | Reserve                                                                    | 00h     | RO         |
+------+----------------------------------------------------------------------------+---------+------------+
| 8:0  | **RD_CNT:**                                                                | 00h     | RO         |
|      |                                                                            |         |            |
|      | Indicate that how many amount of data can be read from the circular queue. |         |            |
+------+----------------------------------------------------------------------------+---------+------------+

### Buffer Length Register (Offset 15h \~14h)

Abbreviation: **BUF_LEN**

Register Index: 15h \~14h

Default Value: 00h

Attribute: RW

+-------+--------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                   | Default | Attributes |
+:======+:===============================================================================+:========+:===========+
| 15:10 | Reserve                                                                        | 00h     | RO         |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 9:0   | **BUF_LEN:**                                                                   | 00h     | RW         |
|       |                                                                                |         |            |
|       | Indicate that total available data number in the circular queue.               |         |            |
|       |                                                                                |         |            |
|       | 1\. 0,1 = forbidden                                                            |         |            |
|       |                                                                                |         |            |
|       | 2 = 2 Data Number (total size of queue = DATA_SIZE \* 2)                       |         |            |
|       |                                                                                |         |            |
|       | ..........                                                                     |         |            |
|       |                                                                                |         |            |
|       | 2\. Range of BUF_LEN :                                                         |         |            |
|       |                                                                                |         |            |
|       | a\. DATA_SIZE = 0 (8Byte) , 2 \<= BUF_LEN \<= 512                              |         |            |
|       |                                                                                |         |            |
|       | b\. DATA_SIZE = 1 (16Byte) , 2 \<= BUF_LEN \<= 256                             |         |            |
|       |                                                                                |         |            |
|       | c\. DATA_SIZE = 2 (32Byte) , 2 \<= BUF_LEN \<= 128                             |         |            |
|       |                                                                                |         |            |
|       | d\. DATA_SIZE = 3 (64Byte) , 2 \<= BUF_LEN \<= 64                              |         |            |
|       |                                                                                |         |            |
|       | e\. DATA_SIZE = 4 (128Byte) , 2 \<= BUF_LEN \<= 32                             |         |            |
|       |                                                                                |         |            |
|       | 3\. While VQ = 1, BUF_LEN is locked and can't be modified.                     |         |            |
|       |                                                                                |         |            |
|       | 4\. Only When VQ = 0 and low byte of BUF_LEN is written, WR_CNT = BUF_LEN - 1. |         |            |
+-------+--------------------------------------------------------------------------------+---------+------------+

### Data Size Register (Offset 16h)

Abbreviation: **DATA_SIZE**

Register Index: 16h

Default Value: 00h

Attribute: RW

+-----+--------------------------------------------------------------------+---------+------------+
| Bit | Descriptions                                                       | Default | Attributes |
+:====+:===================================================================+:========+:===========+
| 7:3 | Reserve                                                            | 00h     | RO         |
+-----+--------------------------------------------------------------------+---------+------------+
| 2:0 | **DATA_SIZE :**                                                    | 00h     | RW         |
|     |                                                                    |         |            |
|     | 1\. Indicate that data size of each element in the circular queue. |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 0 : 1Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 1 : 2Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 2 : 4Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 3 : 8Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 4 : 16Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 5 : 32Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 6 : 64Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 7 : 128Byte                                            |         |            |
|     |                                                                    |         |            |
|     | 2\. While VQ = 1, DATA_SIZE is locked and can't be modified.       |         |            |
+-----+--------------------------------------------------------------------+---------+------------+

### Control Register (Offset 17h)

Abbreviation: **CTRL**

Register Index: 17h

Default Value: 08h

Attribute: RW

+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit | Descriptions                                                                                                     | Default | Attributes |
+:====+:=================================================================================================================+:========+:===========+
| 7:4 | Reserve                                                                                                          | 00h     | RO         |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3   | EMPTY :                                                                                                          | 01h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is empty and no data can be read from the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2   | FULL :                                                                                                           | 00h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is full and data can't be pushed into the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1   | RQ : Reset Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | When RQ = 1, the following events occur.                                                                         |         |            |
|     |                                                                                                                  |         |            |
|     | a\) VQ = 0                                                                                                       |         |            |
|     |                                                                                                                  |         |            |
|     | b\) RD_CT = 0                                                                                                    |         |            |
|     |                                                                                                                  |         |            |
|     | c\) QBASE_ADR = WPTR = RPTR                                                                                      |         |            |
|     |                                                                                                                  |         |            |
|     | d\) WR_CNT = BUF_LEN -1                                                                                          |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0   | VQ : Valid Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | Only when VQ = 1, QBASE_ADR, WPTR, RPTR, BUF_LEN, DATA_SIZE are valid and FW can start to access circular queue. |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+

## Circular Queue 1 (DMAC Completion Queue)

### Queue Base Address Register (Offset 23h\~20h)

Abbreviation: **QBASE_ADR**

Register Index: 23h\~20h

Default Value: 00h

Attribute: RW

+------+-----------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                | Default | Attributes |
+:=====+:============================================================================+:========+:===========+
| 31:0 | **QBASE_ADR :**                                                             | 00h     | RW         |
|      |                                                                             |         |            |
|      | 1\. QBASE_ADR must be 8 byte aligned address.                               |         |            |
|      |                                                                             |         |            |
|      | 2\. While VQ = 0, setting QBASE_ADR will result in QBASE_ADR = WPTR = RPTR. |         |            |
|      |                                                                             |         |            |
|      | 3\. While VQ = 1, QBASE_ADR is locked and can't be modified.                |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

### Write Pointer Register (Offset 27h\~24h)

Abbreviation: **WPTR**

Register Index: 27h\~24h

Default Value: 00h

Attribute: RO

  ---------------------------------------------------------------------------------
  Bit    Descriptions                                        Default   Attributes
  ------ --------------------------------------------------- --------- ------------
  31:0   Write pointer of circular queue.                    00h       RO

  ---------------------------------------------------------------------------------

### Read Pointer Register (Offset 2Bh\~28h)

Abbreviation: **RPTR**

Register Index: 2Bh\~28h

Default Value: 00h

Attribute: RO

  ---------------------------------------------------------------------------------
  Bit    Descriptions                                        Default   Attributes
  ------ --------------------------------------------------- --------- ------------
  31:0   Read pointer of circular queue.                     00h       RO

  ---------------------------------------------------------------------------------

### Write Pointer Increase Unit Register (Offset 2Dh\~2Ch)

Abbreviation: **WPIU**

Register Index: 2D\~2Ch

Default Value: 00h

Attribute: RW

+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                                              | Default | Attributes |
+:=====+:==========================================================================================================+:========+:===========+
| 15:9 | Reserve                                                                                                   | 00h     | RO         |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| 8:0  | **WPIU: (**DMAC triggers this field to maintain WPTR**)**                                                 | 00h     | RW         |
|      |                                                                                                           |         |            |
|      | 1\. Range of WPIU : 1 \~ (BUF_LEN-1)                                                                      |         |            |
|      |                                                                                                           |         |            |
|      | 2\. Only When VQ = 1 and low byte of WPIU is written, WPTR is incremented depending on the following rule |         |            |
|      |                                                                                                           |         |            |
|      | a\. WPTR = WPTR + (DATA_SIZE \* WPIU)                                                                     |         |            |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+

### Writable Count Register (Offset 2Fh\~2Eh)

Abbreviation: **WR_CNT**

Register Index: 2Fh \~ 2Eh

Default Value: 00h

Attribute: RO

+------+-----------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                | Default | Attributes |
+:=====+:============================================================================+:========+:===========+
| 15:9 | Reserve                                                                     | 00h     | RO         |
+------+-----------------------------------------------------------------------------+---------+------------+
| 8:0  | **WR_CNT:**                                                                 | 00h     | RO         |
|      |                                                                             |         |            |
|      | Indicate that how many amount of data can be written to the circular queue. |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

### Read Pointer Increase Unit Register (Offset 31h\~30h)

Abbreviation: **RPIU**

Register Index: 31h\~30h

Default Value: 00h

Attribute: RW

+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                                              | Default | Attributes |
+:=====+:==========================================================================================================+:========+:===========+
| 15:9 | Reserve                                                                                                   | 00h     | RO         |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| 8:0  | **RPIU: (**FW triggers this field to maintain RPTR**)**                                                   | 00h     | RW         |
|      |                                                                                                           |         |            |
|      | 1\. Range of RPIU : 1 \~ (BUF_LEN-1)                                                                      |         |            |
|      |                                                                                                           |         |            |
|      | 2\. Only When VQ = 1 and low byte of RPIU is written, RPTR is incremented depending on the following rule |         |            |
|      |                                                                                                           |         |            |
|      | a\. RPTR = RPTR + (DATA_SIZE \* RPIU)                                                                     |         |            |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+

### Readable Count Register (Offset 33h\~32h)

Abbreviation: **RD_CNT**

Register Index: 33h\~32h

Default Value: 00h

Attribute: RO

+------+----------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                               | Default | Attributes |
+:=====+:===========================================================================+:========+:===========+
| 15:9 | Reserve                                                                    | 00h     | RO         |
+------+----------------------------------------------------------------------------+---------+------------+
| 8:0  | **RD_CNT:**                                                                | 00h     | RO         |
|      |                                                                            |         |            |
|      | Indicate that how many amount of data can be read from the circular queue. |         |            |
+------+----------------------------------------------------------------------------+---------+------------+

### Buffer Length Register (Offset 35h \~34h)

Abbreviation: **BUF_LEN**

Register Index: 35h \~34h

Default Value: 00h

Attribute: RW

+-------+--------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                   | Default | Attributes |
+:======+:===============================================================================+:========+:===========+
| 15:10 | Reserve                                                                        | 00h     | RO         |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 9:0   | **BUF_LEN:**                                                                   | 00h     | RW         |
|       |                                                                                |         |            |
|       | Indicate that total available data number in the circular queue.               |         |            |
|       |                                                                                |         |            |
|       | 1\. 0,1 = forbidden                                                            |         |            |
|       |                                                                                |         |            |
|       | 2 = 2 Data Number (total size of queue = DATA_SIZE \* 2)                       |         |            |
|       |                                                                                |         |            |
|       | ..........                                                                     |         |            |
|       |                                                                                |         |            |
|       | 2\. Range of BUF_LEN :                                                         |         |            |
|       |                                                                                |         |            |
|       | a\. DATA_SIZE = 0 (8Byte) , 2 \<= BUF_LEN \<= 512                              |         |            |
|       |                                                                                |         |            |
|       | b\. DATA_SIZE = 1 (16Byte) , 2 \<= BUF_LEN \<= 256                             |         |            |
|       |                                                                                |         |            |
|       | c\. DATA_SIZE = 2 (32Byte) , 2 \<= BUF_LEN \<= 128                             |         |            |
|       |                                                                                |         |            |
|       | d\. DATA_SIZE = 3 (64Byte) , 2 \<= BUF_LEN \<= 64                              |         |            |
|       |                                                                                |         |            |
|       | e\. DATA_SIZE = 4 (128Byte) , 2 \<= BUF_LEN \<= 32                             |         |            |
|       |                                                                                |         |            |
|       | 3\. While VQ = 1, BUF_LEN is locked and can't be modified.                     |         |            |
|       |                                                                                |         |            |
|       | 4\. Only When VQ = 0 and low byte of BUF_LEN is written, WR_CNT = BUF_LEN - 1. |         |            |
+-------+--------------------------------------------------------------------------------+---------+------------+

### Data Size Register (Offset 36h)

Abbreviation: **DATA_SIZE**

Register Index: 36h

Default Value: 00h

Attribute: RW

+-----+--------------------------------------------------------------------+---------+------------+
| Bit | Descriptions                                                       | Default | Attributes |
+:====+:===================================================================+:========+:===========+
| 7:3 | Reserve                                                            | 00h     | RO         |
+-----+--------------------------------------------------------------------+---------+------------+
| 2:0 | **DATA_SIZE :**                                                    | 00h     | RW         |
|     |                                                                    |         |            |
|     | 1\. Indicate that data size of each element in the circular queue. |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 0 : 1Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 1 : 2Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 2 : 4Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 3 : 8Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 4 : 16Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 5 : 32Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 6 : 64Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 7 : 128Byte                                            |         |            |
|     |                                                                    |         |            |
|     | 2\. While VQ = 1, DATA_SIZE is locked and can't be modified.       |         |            |
+-----+--------------------------------------------------------------------+---------+------------+

### Control Register (Offset 37h)

Abbreviation: **CTRL**

Register Index: 37h

Default Value: 08h

Attribute: RW

+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit | Descriptions                                                                                                     | Default | Attributes |
+:====+:=================================================================================================================+:========+:===========+
| 7:4 | Reserve                                                                                                          | 00h     | RO         |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3   | EMPTY :                                                                                                          | 01h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is empty and no data can be read from the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2   | FULL :                                                                                                           | 00h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is full and data can't be pushed into the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1   | RQ : Reset Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | When RQ = 1, the following events occur.                                                                         |         |            |
|     |                                                                                                                  |         |            |
|     | a\) VQ = 0                                                                                                       |         |            |
|     |                                                                                                                  |         |            |
|     | b\) RD_CT = 0                                                                                                    |         |            |
|     |                                                                                                                  |         |            |
|     | c\) QBASE_ADR = WPTR = RPTR                                                                                      |         |            |
|     |                                                                                                                  |         |            |
|     | d\) WR_CNT = BUF_LEN -1                                                                                          |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0   | VQ : Valid Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | Only when VQ = 1, QBASE_ADR, WPTR, RPTR, BUF_LEN, DATA_SIZE are valid and FW can start to access circular queue. |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+

## Circular Queue 2 (SS Submission Queue)

### Queue Base Address Register (Offset 43h\~40h)

Abbreviation: **QBASE_ADR**

Register Index: 43h\~40h

Default Value: 00h

Attribute: RW

+------+-----------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                | Default | Attributes |
+:=====+:============================================================================+:========+:===========+
| 31:0 | **QBASE_ADR :**                                                             | 00h     | RW         |
|      |                                                                             |         |            |
|      | 1\. QBASE_ADR must be 8 byte aligned address.                               |         |            |
|      |                                                                             |         |            |
|      | 2\. While VQ = 0, setting QBASE_ADR will result in QBASE_ADR = WPTR = RPTR. |         |            |
|      |                                                                             |         |            |
|      | 3\. While VQ = 1, QBASE_ADR is locked and can't be modified.                |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

### Write Pointer Register (Offset 47h\~44h)

Abbreviation: **WPTR**

Register Index: 47h\~44h

Default Value: 00h

Attribute: RO

  ---------------------------------------------------------------------------------
  Bit    Descriptions                                        Default   Attributes
  ------ --------------------------------------------------- --------- ------------
  31:0   Write pointer of circular queue.                    00h       RO

  ---------------------------------------------------------------------------------

### Read Pointer Register (Offset 4Bh\~48h)

Abbreviation: **RPTR**

Register Index: 4Bh\~48h

Default Value: 00h

Attribute: RO

  ---------------------------------------------------------------------------------
  Bit    Descriptions                                        Default   Attributes
  ------ --------------------------------------------------- --------- ------------
  31:0   Read pointer of circular queue.                     00h       RO

  ---------------------------------------------------------------------------------

### Write Pointer Increase Unit Register (Offset 4Dh\~4Ch)

Abbreviation: **WPIU**

Register Index: 4Dh\~4Ch

Default Value: 00h

Attribute: RW

+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                                              | Default | Attributes |
+:=====+:==========================================================================================================+:========+:===========+
| 15:9 | Reserve                                                                                                   | 00h     | RO         |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| 8:0  | **WPIU: (**FW triggers this field to maintain WPTR**)**                                                   | 00h     | RW         |
|      |                                                                                                           |         |            |
|      | 1\. Range of WPIU : 1 \~ (BUF_LEN-1)                                                                      |         |            |
|      |                                                                                                           |         |            |
|      | 2\. Only When VQ = 1 and low byte of WPIU is written, WPTR is incremented depending on the following rule |         |            |
|      |                                                                                                           |         |            |
|      | a\. WPTR = WPTR + (DATA_SIZE \* WPIU)                                                                     |         |            |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+

### Writable Count Register (Offset 4Fh\~4Eh)

Abbreviation: **WR_CNT**

Register Index: 4Fh \~ 4Eh

Default Value: 00h

Attribute: RO

+------+-----------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                | Default | Attributes |
+:=====+:============================================================================+:========+:===========+
| 15:9 | Reserve                                                                     | 00h     | RO         |
+------+-----------------------------------------------------------------------------+---------+------------+
| 8:0  | **WR_CNT:**                                                                 | 00h     | RO         |
|      |                                                                             |         |            |
|      | Indicate that how many amount of data can be written to the circular queue. |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

### Read Pointer Increase Unit Register (Offset 51h\~50h)

Abbreviation: **RPIU**

Register Index: 51h\~50h

Default Value: 00h

Attribute: RW

+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                                              | Default | Attributes |
+:=====+:==========================================================================================================+:========+:===========+
| 15:9 | Reserve                                                                                                   | 00h     | RO         |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| 8:0  | **RPIU: (**SS triggers this field to maintain RPTR**)**                                                   | 00h     | RW         |
|      |                                                                                                           |         |            |
|      | 1\. Range of RPIU : 1 \~ (BUF_LEN-1)                                                                      |         |            |
|      |                                                                                                           |         |            |
|      | 2\. Only When VQ = 1 and low byte of RPIU is written, RPTR is incremented depending on the following rule |         |            |
|      |                                                                                                           |         |            |
|      | a\. RPTR = RPTR + (DATA_SIZE \* RPIU)                                                                     |         |            |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+

### Readable Count Register (Offset 53h\~52h)

Abbreviation: **RD_CNT**

Register Index: 53h\~52h

Default Value: 00h

Attribute: RO

+------+----------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                               | Default | Attributes |
+:=====+:===========================================================================+:========+:===========+
| 15:9 | Reserve                                                                    | 00h     | RO         |
+------+----------------------------------------------------------------------------+---------+------------+
| 8:0  | **RD_CNT:**                                                                | 00h     | RO         |
|      |                                                                            |         |            |
|      | Indicate that how many amount of data can be read from the circular queue. |         |            |
+------+----------------------------------------------------------------------------+---------+------------+

### Buffer Length Register (Offset 55h \~54h)

Abbreviation: **BUF_LEN**

Register Index: 55h \~54h

Default Value: 00h

Attribute: RW

+-------+--------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                   | Default | Attributes |
+:======+:===============================================================================+:========+:===========+
| 15:10 | Reserve                                                                        | 00h     | RO         |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 9:0   | **BUF_LEN:**                                                                   | 00h     | RW         |
|       |                                                                                |         |            |
|       | Indicate that total available data number in the circular queue.               |         |            |
|       |                                                                                |         |            |
|       | 1\. 0,1 = forbidden                                                            |         |            |
|       |                                                                                |         |            |
|       | 2 = 2 Data Number (total size of queue = DATA_SIZE \* 2)                       |         |            |
|       |                                                                                |         |            |
|       | ..........                                                                     |         |            |
|       |                                                                                |         |            |
|       | 2\. Range of BUF_LEN :                                                         |         |            |
|       |                                                                                |         |            |
|       | a\. DATA_SIZE = 0 (8Byte) , 2 \<= BUF_LEN \<= 512                              |         |            |
|       |                                                                                |         |            |
|       | b\. DATA_SIZE = 1 (16Byte) , 2 \<= BUF_LEN \<= 256                             |         |            |
|       |                                                                                |         |            |
|       | c\. DATA_SIZE = 2 (32Byte) , 2 \<= BUF_LEN \<= 128                             |         |            |
|       |                                                                                |         |            |
|       | d\. DATA_SIZE = 3 (64Byte) , 2 \<= BUF_LEN \<= 64                              |         |            |
|       |                                                                                |         |            |
|       | e\. DATA_SIZE = 4 (128Byte) , 2 \<= BUF_LEN \<= 32                             |         |            |
|       |                                                                                |         |            |
|       | 3\. While VQ = 1, BUF_LEN is locked and can't be modified.                     |         |            |
|       |                                                                                |         |            |
|       | 4\. Only When VQ = 0 and low byte of BUF_LEN is written, WR_CNT = BUF_LEN - 1. |         |            |
+-------+--------------------------------------------------------------------------------+---------+------------+

### Data Size Register (Offset 56h)

Abbreviation: **DATA_SIZE**

Register Index: 56h

Default Value: 00h

Attribute: RW

+-----+--------------------------------------------------------------------+---------+------------+
| Bit | Descriptions                                                       | Default | Attributes |
+:====+:===================================================================+:========+:===========+
| 7:3 | Reserve                                                            | 00h     | RO         |
+-----+--------------------------------------------------------------------+---------+------------+
| 2:0 | **DATA_SIZE :**                                                    | 00h     | RW         |
|     |                                                                    |         |            |
|     | 1\. Indicate that data size of each element in the circular queue. |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 0 : 1Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 1 : 2Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 2 : 4Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 3 : 8Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 4 : 16Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 5 : 32Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 6 : 64Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 7 : 128Byte                                            |         |            |
|     |                                                                    |         |            |
|     | 2\. While VQ = 1, DATA_SIZE is locked and can't be modified.       |         |            |
+-----+--------------------------------------------------------------------+---------+------------+

### Control Register (Offset 57h)

Abbreviation: **CTRL**

Register Index: 57h

Default Value: 08h

Attribute: RW

+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit | Descriptions                                                                                                     | Default | Attributes |
+:====+:=================================================================================================================+:========+:===========+
| 7:4 | Reserve                                                                                                          | 00h     | RO         |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3   | EMPTY :                                                                                                          | 01h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is empty and no data can be read from the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2   | FULL :                                                                                                           | 00h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is full and data can't be pushed into the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1   | RQ : Reset Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | When RQ = 1, the following events occur.                                                                         |         |            |
|     |                                                                                                                  |         |            |
|     | a\) VQ = 0                                                                                                       |         |            |
|     |                                                                                                                  |         |            |
|     | b\) RD_CT = 0                                                                                                    |         |            |
|     |                                                                                                                  |         |            |
|     | c\) QBASE_ADR = WPTR = RPTR                                                                                      |         |            |
|     |                                                                                                                  |         |            |
|     | d\) WR_CNT = BUF_LEN -1                                                                                          |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0   | VQ : Valid Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | Only when VQ = 1, QBASE_ADR, WPTR, RPTR, BUF_LEN, DATA_SIZE are valid and FW can start to access circular queue. |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+

## Circular Queue 3 (SS Completion Queue)

### Queue Base Address Register (Offset 63h\~60h)

Abbreviation: **QBASE_ADR**

Register Index: 63h\~60h

Default Value: 00h

Attribute: RW

+------+-----------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                | Default | Attributes |
+:=====+:============================================================================+:========+:===========+
| 31:0 | **QBASE_ADR :**                                                             | 00h     | RW         |
|      |                                                                             |         |            |
|      | 1\. QBASE_ADR must be 8 byte aligned address.                               |         |            |
|      |                                                                             |         |            |
|      | 2\. While VQ = 0, setting QBASE_ADR will result in QBASE_ADR = WPTR = RPTR. |         |            |
|      |                                                                             |         |            |
|      | 3\. While VQ = 1, QBASE_ADR is locked and can't be modified.                |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

### Write Pointer Register (Offset 67h\~64h)

Abbreviation: **WPTR**

Register Index: 67h\~64h

Default Value: 00h

Attribute: RO

  ---------------------------------------------------------------------------------
  Bit    Descriptions                                        Default   Attributes
  ------ --------------------------------------------------- --------- ------------
  31:0   Write pointer of circular queue.                    00h       RO

  ---------------------------------------------------------------------------------

### Read Pointer Register (Offset 6Bh\~68h)

Abbreviation: **RPTR**

Register Index: 6Bh\~68h

Default Value: 00h

Attribute: RO

  ---------------------------------------------------------------------------------
  Bit    Descriptions                                        Default   Attributes
  ------ --------------------------------------------------- --------- ------------
  31:0   Read pointer of circular queue.                     00h       RO

  ---------------------------------------------------------------------------------

### Write Pointer Increase Unit Register (Offset 6Dh\~6Ch)

Abbreviation: **WPIU**

Register Index: 6Dh\~6Ch

Default Value: 00h

Attribute: RW

+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                                              | Default | Attributes |
+:=====+:==========================================================================================================+:========+:===========+
| 15:9 | Reserve                                                                                                   | 00h     | RO         |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| 8:0  | **WPIU: (**SS triggers this field to maintain WPTR**)**                                                   | 00h     | RW         |
|      |                                                                                                           |         |            |
|      | 1\. Range of WPIU : 1 \~ (BUF_LEN-1)                                                                      |         |            |
|      |                                                                                                           |         |            |
|      | 2\. Only When VQ = 1 and low byte of WPIU is written, WPTR is incremented depending on the following rule |         |            |
|      |                                                                                                           |         |            |
|      | a\. WPTR = WPTR + (DATA_SIZE \* WPIU)                                                                     |         |            |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+

### Writable Count Register (Offset 6Fh\~6Eh)

Abbreviation: **WR_CNT**

Register Index: 6Fh \~ 6Eh

Default Value: 00h

Attribute: RO

+------+-----------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                | Default | Attributes |
+:=====+:============================================================================+:========+:===========+
| 15:9 | Reserve                                                                     | 00h     | RO         |
+------+-----------------------------------------------------------------------------+---------+------------+
| 8:0  | **WR_CNT:**                                                                 | 00h     | RO         |
|      |                                                                             |         |            |
|      | Indicate that how many amount of data can be written to the circular queue. |         |            |
+------+-----------------------------------------------------------------------------+---------+------------+

### Read Pointer Increase Unit Register (Offset 71h\~70h)

Abbreviation: **RPIU**

Register Index: 71h\~70h

Default Value: 00h

Attribute: RW

+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                                                              | Default | Attributes |
+:=====+:==========================================================================================================+:========+:===========+
| 15:9 | Reserve                                                                                                   | 00h     | RO         |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+
| 8:0  | **RPIU: (**FW triggers this field to maintain RPTR**)**                                                   | 00h     | RW         |
|      |                                                                                                           |         |            |
|      | 1\. Range of RPIU : 1 \~ (BUF_LEN-1)                                                                      |         |            |
|      |                                                                                                           |         |            |
|      | 2\. Only When VQ = 1 and low byte of RPIU is written, RPTR is incremented depending on the following rule |         |            |
|      |                                                                                                           |         |            |
|      | a\. RPTR = RPTR + (DATA_SIZE \* RPIU)                                                                     |         |            |
+------+-----------------------------------------------------------------------------------------------------------+---------+------------+

### Readable Count Register (Offset 73h\~72h)

Abbreviation: **RD_CNT**

Register Index: 73h\~72h

Default Value: 00h

Attribute: RO

+------+----------------------------------------------------------------------------+---------+------------+
| Bit  | Descriptions                                                               | Default | Attributes |
+:=====+:===========================================================================+:========+:===========+
| 15:9 | Reserve                                                                    | 00h     | RO         |
+------+----------------------------------------------------------------------------+---------+------------+
| 8:0  | **RD_CNT:**                                                                | 00h     | RO         |
|      |                                                                            |         |            |
|      | Indicate that how many amount of data can be read from the circular queue. |         |            |
+------+----------------------------------------------------------------------------+---------+------------+

### Buffer Length Register (Offset 75h \~74h)

Abbreviation: **BUF_LEN**

Register Index: 75h \~74h

Default Value: 00h

Attribute: RW

+-------+--------------------------------------------------------------------------------+---------+------------+
| Bit   | Descriptions                                                                   | Default | Attributes |
+:======+:===============================================================================+:========+:===========+
| 15:10 | Reserve                                                                        | 00h     | RO         |
+-------+--------------------------------------------------------------------------------+---------+------------+
| 9:0   | **BUF_LEN:**                                                                   | 00h     | RW         |
|       |                                                                                |         |            |
|       | Indicate that total available data number in the circular queue.               |         |            |
|       |                                                                                |         |            |
|       | 1\. 0,1 = forbidden                                                            |         |            |
|       |                                                                                |         |            |
|       | 2 = 2 Data Number (total size of queue = DATA_SIZE \* 2)                       |         |            |
|       |                                                                                |         |            |
|       | ..........                                                                     |         |            |
|       |                                                                                |         |            |
|       | 2\. Range of BUF_LEN :                                                         |         |            |
|       |                                                                                |         |            |
|       | a\. DATA_SIZE = 0 (8Byte) , 2 \<= BUF_LEN \<= 512                              |         |            |
|       |                                                                                |         |            |
|       | b\. DATA_SIZE = 1 (16Byte) , 2 \<= BUF_LEN \<= 256                             |         |            |
|       |                                                                                |         |            |
|       | c\. DATA_SIZE = 2 (32Byte) , 2 \<= BUF_LEN \<= 128                             |         |            |
|       |                                                                                |         |            |
|       | d\. DATA_SIZE = 3 (64Byte) , 2 \<= BUF_LEN \<= 64                              |         |            |
|       |                                                                                |         |            |
|       | e\. DATA_SIZE = 4 (128Byte) , 2 \<= BUF_LEN \<= 32                             |         |            |
|       |                                                                                |         |            |
|       | 3\. While VQ = 1, BUF_LEN is locked and can't be modified.                     |         |            |
|       |                                                                                |         |            |
|       | 4\. Only When VQ = 0 and low byte of BUF_LEN is written, WR_CNT = BUF_LEN - 1. |         |            |
+-------+--------------------------------------------------------------------------------+---------+------------+

### Data Size Register (Offset 76h)

Abbreviation: **DATA_SIZE**

Register Index: 76h

Default Value: 00h

Attribute: RW

+-----+--------------------------------------------------------------------+---------+------------+
| Bit | Descriptions                                                       | Default | Attributes |
+:====+:===================================================================+:========+:===========+
| 7:3 | Reserve                                                            | 00h     | RO         |
+-----+--------------------------------------------------------------------+---------+------------+
| 2:0 | **DATA_SIZE :**                                                    | 00h     | RW         |
|     |                                                                    |         |            |
|     | 1\. Indicate that data size of each element in the circular queue. |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 0 : 1Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 1 : 2Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 2 : 4Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 3 : 8Byte                                              |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 4 : 16Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 5 : 32Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 6 : 64Byte                                             |         |            |
|     |                                                                    |         |            |
|     | DATA_SIZE = 7 : 128Byte                                            |         |            |
|     |                                                                    |         |            |
|     | 2\. While VQ = 1, DATA_SIZE is locked and can't be modified.       |         |            |
+-----+--------------------------------------------------------------------+---------+------------+

### Control Register (Offset 77h)

Abbreviation: **CTRL**

Register Index: 77h

Default Value: 08h

Attribute: RW

+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| Bit | Descriptions                                                                                                     | Default | Attributes |
+:====+:=================================================================================================================+:========+:===========+
| 7:4 | Reserve                                                                                                          | 00h     | RO         |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 3   | EMPTY :                                                                                                          | 01h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is empty and no data can be read from the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 2   | FULL :                                                                                                           | 00h     | RO         |
|     |                                                                                                                  |         |            |
|     | Indicate circular queue is full and data can't be pushed into the circular queue.                                |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 1   | RQ : Reset Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | When RQ = 1, the following events occur.                                                                         |         |            |
|     |                                                                                                                  |         |            |
|     | a\) VQ = 0                                                                                                       |         |            |
|     |                                                                                                                  |         |            |
|     | b\) RD_CT = 0                                                                                                    |         |            |
|     |                                                                                                                  |         |            |
|     | c\) QBASE_ADR = WPTR = RPTR                                                                                      |         |            |
|     |                                                                                                                  |         |            |
|     | d\) WR_CNT = BUF_LEN -1                                                                                          |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+
| 0   | VQ : Valid Queue                                                                                                 | 00h     | RW         |
|     |                                                                                                                  |         |            |
|     | Only when VQ = 1, QBASE_ADR, WPTR, RPTR, BUF_LEN, DATA_SIZE are valid and FW can start to access circular queue. |         |            |
+-----+------------------------------------------------------------------------------------------------------------------+---------+------------+

# DMAC Design Spec (QBody)

## OP Code (1 Unit)

  ---------------------------------------------------------------------------------
  SQ         **COPY**   **SV**   **E3D**   **PZIP**   **XOR**   **CMP**   **CRC**
  ---------- ---------- -------- --------- ---------- --------- --------- ---------
  OP0 (4B)   CMD        CMD      CMD       CMD        CMD       CMD       CMD

  OP1 (4B)   SADD       X        SADD      SADD       S1ADD     S1ADD     S1ADD

  OP2 (4B)   TADD       TADD     X         X          S2ADD     S2ADD     X

  OP3 (4B)   X          VAL      X         X          TADD      X         VAL

  OP4 (4B)   LEN        LEN      LEN       X          LEN       LEN       LEN

  OP5 (4B)   MSK        MSK      MSK       X          X         X         X

  OP6 (4B)   LBA        LBA      LBA       LBA        X         X         X

  OP7 (4B)   X          X        X         X          X         X         X

  CQ         **COPY**   **SV**   **E3D**   **ZIP**    **XOR**   **CMP**   **CRC**

  OP0 (8B)   STA        STA      STA       STA        STA       STA       STA
  ---------------------------------------------------------------------------------

## OP Discription For Each Command

### Copy mode

+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
| OP Symbol         | Bit Map           | Bit Symbol        | COPY              | Description                                                                |
+===================+===================+===================+===================+============================================================================+
| CMD               | 7 \~ 0            | OP                | 8\'h0             | 8\'h0                                                                      |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
|                   | 9 \~ 8            | SIZ_UNIT          | 2\'h0             | Fix to Byte Access.                                                        |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
|                   | 10                | SEL\_\            | 1\'b0             | don\'t care                                                                |
|                   |                   | XOR_XNOR          |                   |                                                                            |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
|                   | 11                | MSK_EN            | Dep               | Depend On FW.                                                              |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
|                   | 12                | RND_EN            | 1\'b0             | don\'t care                                                                |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
|                   | 13                | Reserved          | 1\'b0             | don\'t care                                                                |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
|                   | 14                | ERR_INS           | Dep               | Depend On FW.                                                              |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
|                   | 15                | SET_CQ            | Dep               | 1\'h0: don\'t set status information to CQ\                                |
|                   |                   |                   |                   | 1\'h1: need set status information to CQ                                   |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
|                   | 23 \~ 16          | SQ_TAG            | Dep               | Depend On FW.                                                              |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
|                   | 31 \~ 24          | OP_LEN            | 8\'h8             | Fix to 8 OP.                                                               |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
| SADD              | 31 \~ 0           | SADD              | Dep               | Source Address.                                                            |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
| TADD              | 31 \~ 0           | TADD              | Dep               | Target Address.                                                            |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
| VAL               | 31 \~ 0           | Value             | 32\'hx            | don\'t care　                                                              |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
| LEN               | 31 \~ 0           | LEN               | Dep               | Transfer Byte Count.                                                       |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
| MSK               | 31 \~ 0           | SCT_MSK           | Dep               | Depend On FW.                                                              |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
| LBA               | 31 \~ 0           | LBA               | Dep               | LBA value to generate E3D. (used in copying form non-E3D area to E3D area) |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
| RSV               | 31\~0             | RSV               | 32\'hx            | don\'t care                                                                |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
| CQ                                                                                                                                                         |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
| STA               | 7 \~ 0            | STA               | Dep               | 8\'h0: Done.                                                               |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
|                   | 15 \~ 8           | CQ_TAG            | Dep               | Depend On FW.                                                              |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
|                   | 31 \~ 16          | RSV               | 16\'h0            | don\'t care                                                                |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------------+
|                   | 63 \~ 32          | STA               | 32\'h0            | don\'t care                                                                |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------------+

### SV mode

+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
| OP Symbol         | Bit Map           | Bit Symbol        | SV                | Description                                                                  |
+===================+===================+===================+===================+==============================================================================+
| CMD               | 7 \~ 0            | OP                | 8\'h1             | 8\'h1                                                                        |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
|                   | 9 \~ 8            | SIZ_UNIT          | Dep               | RND_EN is \'0\':\                                                            |
|                   |                   |                   |                   | Set Value unit is 1B, 2B or 4B.\                                             |
|                   |                   |                   |                   | RND_EN is \'1\':\                                                            |
|                   |                   |                   |                   | don\'t care this setting; Set Value unit is fixed to 2B.                     |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
|                   | 10                | SEL\_\            | 1\'b0             | don\'t care                                                                  |
|                   |                   | XOR_XNOR          |                   |                                                                              |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
|                   | 11                | MSK_EN            | Dep               | Depend On FW.                                                                |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
|                   | 12                | RND_EN            | Dep               | 1\'b0:Write VAL to target address.\                                          |
|                   |                   |                   |                   | 1\'b1:Write Random data are generated by LBA(random seed) to target address. |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
|                   | 13                | Reserved          | 1\'b0             | don\'t care                                                                  |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
|                   | 14                | ERR_INS           | 1\'b0             | don\'t care                                                                  |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
|                   | 15                | SET_CQ            | Dep               | 1\'h0: don\'t set status information to CQ\                                  |
|                   |                   |                   |                   | 1\'h1: need set status information to CQ                                     |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
|                   | 23 \~ 16          | SQ_TAG            | Dep               | Depend On FW.                                                                |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
|                   | 31 \~ 24          | OP_LEN            | 8\'h8             | Fix to 8 OP.                                                                 |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
| SADD              | 31 \~ 0           | SADD              | 32\'hx            | don\'t care                                                                  |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
| TADD              | 31 \~ 0           | TADD              | Dep               | Target Address.                                                              |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
| VAL               | 31 \~ 0           | Value             | Dep               | Set value to target address. (Only valid when RND_EN is \'0\')               |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
| LEN               | 31 \~ 0           | LEN               | Dep               | Transfer Byte Count.                                                         |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
| MSK               | 31 \~ 0           | SCT_MSK           | Dep               | Depend On FW.　                                                              |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
| LBA               | 31 \~ 0           | LBA               | Dep               | LBA value to generate E3D.                                                   |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
| RSV               | 31\~0             | RSV               | 32\'hx            | don\'t care                                                                  |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
| CQ                                                                                                                                                           |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
| STA               | 7 \~ 0            | STA               | Dep               | 8\'h0: Done.                                                                 |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
|                   | 15 \~ 8           | CQ_TAG            | Dep               | Depend On FW.                                                                |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
|                   | 31 \~ 16          | RSV               | 16\'h0            | don\'t care                                                                  |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------------------+
|                   | 63 \~ 32          | STA               | 32\'h0            | don\'t care                                                                  |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------------------+

**Note**: If you want to create the same random data, the target address LSB 5 bits and LBA value of SQ1 should be equal to which of SQ2.

### E3D mode

+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| OP Symbol         | Bit Map           | Bit Symbol        | E3D               | Description                                 |
+===================+===================+===================+===================+=============================================+
| CMD               | 7 \~ 0            | OP                | 8\'h2             | 8\'h2                                       |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 9 \~ 8            | SIZ_UNIT          | 2\'h1             | Fix to 2B.                                  |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 10                | SEL\_\            | 1\'b0             | don\'t care                                 |
|                   |                   | XOR_XNOR          |                   |                                             |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 11                | MSK_EN            | 1\'b0             | don\'t care                                 |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 12                | RND_EN            | 1\'b0             | don\'t care                                 |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 13                | Reserved          | 1\'b0             | don\'t care                                 |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 14                | ERR_INS           | 1\'b0             | don\'t care                                 |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 15                | SET_CQ            | Dep               | 1\'h0: don\'t set status information to CQ\ |
|                   |                   |                   |                   | 1\'h1: need set status information to CQ    |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 23 \~ 16          | SQ_TAG            | Dep               | Depend On FW.                               |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 31 \~ 24          | OP_LEN            | 8\'h8             | Fix to 8 OP.                                |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| SADD              | 31 \~ 0           | SADD              | Dep               | Source Address.                             |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| TADD              | 31 \~ 0           | TADD              | 32\'hx            | don\'t care                                 |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| VAL               | 31 \~ 0           | Value             | 32\'hx            | don\'t care　                               |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| LEN               | 31 \~ 0           | LEN               | 32\'hff           | Fix to 512B.                                |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| MSK               | 31 \~ 0           | SCT_MSK           | 32\'hx            | don\'t care                                 |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| LBA               | 31 \~ 0           | LBA               | Dep               | LBA value to generate E3D.                  |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| RSV               | 31\~0             | RSV               | 32\'hx            | don\'t care                                 |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| CQ                                                                                                                          |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| STA               | 7 \~ 0            | STA               | Dep               | 8\'h0: Done.\                               |
|                   |                   |                   |                   | 8\'h1: E3D check error.                     |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 15 \~ 8           | CQ_TAG            | Dep               | Depend On FW.                               |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 31\~16            | RSV               | Dep               | don't care.                                 |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 63\~32            | STA               | Dep               | Expect E3D data in E3D check error\         |
|                   |                   |                   |                   | Default is 16\'h0 in no E3D check error     |
|                   |                   |                   +-------------------+---------------------------------------------+
|                   |                   |                   | Dep               | Actual E3D data in E3D check error\         |
|                   |                   |                   |                   | Default is 16\'h0 in no E3D check error     |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+

### PZIP mode

+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------+
| OP Symbol         | Bit Map           | Bit Symbol        | PZIP              | Description                                                      |
+===================+===================+===================+===================+==================================================================+
| CMD               | 7 \~ 0            | OP                | 8\'h3             | 8\'h3                                                            |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------+
|                   | 9 \~ 8            | SIZ_UNIT          | 2\'hx             | Fix to 8B.                                                       |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------+
|                   | 10                | SEL\_\            | 1\'b0             | don\'t care                                                      |
|                   |                   | XOR_XNOR          |                   |                                                                  |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------+
|                   | 11                | MSK_EN            | 1\'b0             | don\'t care                                                      |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------+
|                   | 12                | RND_EN            | 1\'b0             | don\'t care                                                      |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------+
|                   | 13                | Reserved          | 1\'b0             | don\'t care                                                      |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------+
|                   | 14                | ERR_INS           | Dep               | Depend On FW.                                                    |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------+
|                   | 15                | SET_CQ            | Dep               | 1\'h0: don\'t set status information to CQ\                      |
|                   |                   |                   |                   | 1\'h1: need set status information to CQ                         |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------+
|                   | 23 \~ 16          | SQ_TAG            | Dep               | Depend On FW.                                                    |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------+
|                   | 31 \~ 24          | OP_LEN            | 8\'h8             | Fix to 8 OP.                                                     |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------+
| SADD              | 31 \~ 0           | SADD              | Dep               | Source Address.　                                                |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------+
| TADD              | 31 \~ 0           | TADD              | 32\'hx            | don\'t care                                                      |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------+
| VAL               | 31 \~ 0           | Value             | 32\'hx            | don\'t care                                                      |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------+
| LEN               | 31 \~ 0           | LEN               | Dep               | Fix to 4KB.                                                      |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------+
| MSK               | 31 \~ 0           | SCT_MSK           | 32\'hx            | don\'t care                                                      |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------+
| LBA               | 31 \~ 0           | LBA               | 32\'hx            | don\'t care                                                      |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------+
| RSV               | 31\~0             | RSV               | 32\'hx            | don\'t care                                                      |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------+
| CQ                                                                                                                                               |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------+
| STA               | 7 \~ 0            | STA               | Dep               | 8\'h0: zdata \<=0.5K, 8\'h1: zdata \<=1K, 8\'h2: zdata \<=1.5K,\ |
|                   |                   |                   |                   | 8\'h3: zdata \<=2K, 8\'h4: zdata \<=2.5K, 8\'h5: zdata \<=3K\    |
|                   |                   |                   |                   | 8\'h6: zdata \<=3.5K, 8\'h7: zdata \>3.5K                        |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------+
|                   | 15 \~ 8           | CQ_TAG            | Dep               | Depend On FW.                                                    |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------+
|                   | 31 \~ 16          | RSV               | 16\'h0            | don\'t care                                                      |
|                   +-------------------+-------------------+-------------------+------------------------------------------------------------------+
|                   | 63 \~ 32          | STA               | 32\'h0            | don\'t care                                                      |
+-------------------+-------------------+-------------------+-------------------+------------------------------------------------------------------+

**Note**: one can tune LZSS resolution in bit0&bit1 of bSPI(0x20) : CR_LZSS,CFG

### XOR mode

+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| OP Symbol         | Bit Map           | Bit Symbol        | XOR               | Description                                 |
+===================+===================+===================+===================+=============================================+
| CMD               | 7 \~ 0            | OP                | 8\'h4             | 8\'h4                                       |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 9 \~ 8            | SIZ_UNIT          | 2\'hx             | Fix to 32B.                                 |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 10                | SEL\_\            | Dep               | select XOR or XNOR mode\                    |
|                   |                   | XOR_XNOR          |                   | 1\'b0: XOR mode\                            |
|                   |                   |                   |                   | 1\'b1: XNOR mode                            |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 11                | MSK_EN            | 1\'b0             | don\'t care                                 |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 12                | RND_EN            | 1\'b0             | don\'t care                                 |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 13                | Reserved          | 1\'b0             | don\'t care                                 |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 14                | ERR_INS           | Dep               | Depend On FW.                               |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 15                | SET_CQ            | Dep               | 1\'h0: don\'t set status information to CQ\ |
|                   |                   |                   |                   | 1\'h1: need set status information to CQ    |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 23 \~ 16          | SQ_TAG            | Dep               | Depend On FW.                               |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 31 \~ 24          | OP_LEN            | 8\'h8             | Fix to 8 OP.                                |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| SADD              | 31 \~ 0           | SADD              | Dep               | Source 1 Address: 512B alignment.           |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| TADD              | 31 \~ 0           | TADD              | Dep               | Source 2 Address: 512B alignment.           |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| VAL               | 31 \~ 0           | Value             | Dep               | Target Address: 512B alignment.             |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| LEN               | 31 \~ 0           | LEN               | Dep               | Transfer Byte Count.                        |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| MSK               | 31 \~ 0           | SCT_MSK           | 32\'hx            | don\'t care                                 |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| LBA               | 31 \~ 0           | LBA               | 32\'hx            | don\'t care                                 |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| RSV               | 31\~0             | RSV               | 32\'hx            | don\'t care                                 |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| CQ                                                                                                                          |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+
| STA               | 7 \~ 0            | STA               | Dep               | 8\'h0: Done.                                |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 15 \~ 8           | CQ_TAG            | Dep               | Depend On FW.                               |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 31 \~ 16          | RSV               | 16\'h0            | don\'t care                                 |
|                   +-------------------+-------------------+-------------------+---------------------------------------------+
|                   | 63 \~ 32          | STA               | 32\'h0            | don\'t care                                 |
+-------------------+-------------------+-------------------+-------------------+---------------------------------------------+

### CMP mode

+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------+
| OP Symbol         | Bit Map           | Bit Symbol        | CMP               | Description                                                          |
+===================+===================+===================+===================+======================================================================+
| CMD               | 7 \~ 0            | OP                | 8\'h5             | 8\'h5                                                                |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------+
|                   | 9 \~ 8            | SIZ_UNIT          | 2\'hx             | Fix to 32B.                                                          |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------+
|                   | 10                | SEL\_\            | 1\'b0             | don\'t care                                                          |
|                   |                   | XOR_XNOR          |                   |                                                                      |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------+
|                   | 11                | MSK_EN            | 1\'b0             | don\'t care                                                          |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------+
|                   | 12                | RND_EN            | 1\'b0             | don\'t care                                                          |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------+
|                   | 13                | Reserved          | 1\'b0             | don\'t care                                                          |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------+
|                   | 14                | ERR_INS           | Dep               | Depend On FW.                                                        |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------+
|                   | 15                | SET_CQ            | Dep               | 1\'h0: don\'t set status information to CQ\                          |
|                   |                   |                   |                   | 1\'h1: need set status information to CQ                             |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------+
|                   | 23 \~ 16          | SQ_TAG            | Dep               | Depend On FW.                                                        |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------+
|                   | 31 \~ 24          | OP_LEN            | 8\'h8             | Fix to 8 OP.                                                         |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------+
| SADD              | 31 \~ 0           | SADD              | Dep               | Source 1 Address.                                                    |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------+
| TADD              | 31 \~ 0           | TADD              | Dep               | Source 2 Address.                                                    |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------+
| VAL               | 31 \~ 0           | Value             | 32\'hx            | don\'t care                                                          |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------+
| LEN               | 31 \~ 0           | LEN               | Dep               | Transfer Byte Count.                                                 |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------+
| MSK               | 31 \~ 0           | SCT_MSK           | 32\'hx            | don\'t care                                                          |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------+
| LBA               | 31 \~ 0           | LBA               | 32\'hx            | don\'t care                                                          |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------+
| RSV               | 31\~0             | RSV               | 32\'hx            | don\'t care                                                          |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------+
| CQ                                                                                                                                                   |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------+
| STA               | 7 \~ 0            | STA               | Dep               | 8\'h0: No Error.\                                                    |
|                   |                   |                   |                   | 8\'h1: Compare Error.                                                |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------+
|                   | 15 \~ 8           | CQ_TAG            | Dep               | Depend On FW.                                                        |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------+
|                   | 31 \~ 16          | RSV               | 16\'h0            | don\'t care                                                          |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------------+
|                   | 63 \~ 32          | STA               | Dep               | Offset Address\                                                      |
|                   |                   |                   |                   | if compare error, SS will return the offset address(truncate to 32B) |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------------+

### CRC mode

+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------+
| OP Symbol         | Bit Map           | Bit Symbol        | CMP               | Description                                                    |
+===================+===================+===================+===================+================================================================+
| CMD               | 7 \~ 0            | OP                | 8\'h5             | 8\'h6                                                          |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------+
|                   | 9 \~ 8            | SIZ_UNIT          | 2\'hx             | Fix to 32B.                                                    |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------+
|                   | 10                | SEL\_\            | Dep               | select CRC32 calculate or check operation                      |
|                   |                   | XOR_XNOR          |                   |                                                                |
|                   |                   |                   |                   | 1\'b0: CRC32 calculate mode                                    |
|                   |                   |                   |                   |                                                                |
|                   |                   |                   |                   | 1\'b1: CRC32 check mode                                        |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------+
|                   | 11                | MSK_EN            | Dep               | At calculate mode: generate CRC32 for ROM or normal pattern\   |
|                   |                   |                   |                   | 1\'h0: for normal pattern(calculate to ByteLength)\            |
|                   |                   |                   |                   | 1\'h1: for ROM(calculate to (ByteLength-8))                    |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------+
|                   | 12                | RND_EN            | Dep               | calculate or check data form ROM or DCCM/DDR/DBUF              |
|                   |                   |                   |                   |                                                                |
|                   |                   |                   |                   | 0: ROM                                                         |
|                   |                   |                   |                   |                                                                |
|                   |                   |                   |                   | 1: DCCM/DDR/DBUF                                               |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------+
|                   | 13                | Reserved          | 1\'b0             | don\'t care                                                    |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------+
|                   | 14                | ERR_INS           | 1\'b0             | don\'t care                                                    |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------+
|                   | 15                | SET_CQ            | Dep               | 1\'h0: don\'t set status information to CQ\                    |
|                   |                   |                   |                   | 1\'h1: need set status information to CQ                       |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------+
|                   | 23 \~ 16          | SQ_TAG            | Dep               | Depend On FW.                                                  |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------+
|                   | 31 \~ 24          | OP_LEN            | 8\'h8             | Fix to 8 OP.                                                   |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------+
| SADD              | 31 \~ 0           | SADD              | Dep               | Source 1 Address.                                              |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------+
| TADD              | 31 \~ 0           | TADD              | 32\'hx            | don\'t care                                                    |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------+
| VAL               | 31 \~ 0           | Value             | Dep               | Initial value                                                  |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------+
| LEN               | 31 \~ 0           | LEN               | Dep               | Transfer Byte Count. (32Byte alignment for DCCM/DDR.Z1/DDR.Z2) |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------+
| MSK               | 31 \~ 0           | SCT_MSK           | 32\'hx            | don\'t care                                                    |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------+
| LBA               | 31 \~ 0           | LBA               | 32\'hx            | don\'t care                                                    |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------+
| RSV               | 31\~0             | RSV               | 32\'hx            | don\'t care                                                    |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------+
| CQ                                                                                                                                             |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------+
| STA               | 7 \~ 0            | STA               | Dep               | at CRC32 calculate mode, 0: done                               |
|                   |                   |                   |                   |                                                                |
|                   |                   |                   |                   | at CRC32 check mode, 0:fail, 1:pass                            |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------+
|                   | 15 \~ 8           | CQ_TAG            | Dep               | Depend On FW.                                                  |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------+
|                   | 31 \~ 16          | RSV               | 16\'h0            | don\'t care                                                    |
|                   +-------------------+-------------------+-------------------+----------------------------------------------------------------+
|                   | 63 \~ 32          | STA               | Dep               | at CRC32 calculate mode: report the CRC32 result               |
|                   |                   |                   |                   |                                                                |
|                   |                   |                   |                   | at CRC32 check mode: don\'t care                               |
+-------------------+-------------------+-------------------+-------------------+----------------------------------------------------------------+

**Note**: In check mode, bit11 of CMD need set to 1 (i.e. calculate to (ByteLength-8))

## DMAC Control Flow and Note

+------------+------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------+
| **Step 1** | **Submission Queue setting and Completion Queue setting**                                                                                            |                                            |
|            |                                                                                                                                                      |                                            |
|            | **(Set QInfo)**                                                                                                                                      |                                            |
|            +------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------+
|            | // SQ setting\                                                                                                                                       | // Submission Queue Base Address\          |
|            | dDRAM0_0(0x00) = 0x5C0C8000;\                                                                                                                        | // Buffer Length (example: 1KB = 32B\*32)\ |
|            | wDRAM0_0(0x14) = 0x20;\                                                                                                                              | // Data Size: 0x02 (32 Byte)\              |
|            | bDRAM0_0(0x16) = 0x05;\                                                                                                                              | // Control: Valid Queue                    |
|            | bDRAM0_0(0x17) = 0x01;                                                                                                                               |                                            |
|            +------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------+
|            | // CQ setting\                                                                                                                                       | // Completion Queue Base Address\          |
|            | dDRAM0_0(0x20) = 0x5C0C9000;\                                                                                                                        | // Buffer Length (example: 1KB = 8B\*128)\ |
|            | wDRAM0_0(0x34) = 0x80;\                                                                                                                              | // Data Size: 0x00 (8 Byte)\               |
|            | bDRAM0_0(0x36) = 0x03;\                                                                                                                              | // Control: Valid Queue                    |
|            | bDRAM0_0(0x37) = 0x01;                                                                                                                               |                                            |
+:===========+:=====================================================================================================================================================+:===========================================+
| **Step 2** | **Putting the command setting to SQ.It can continually put the command setting to SQ until the SQ full.**                                            |                                            |
|            |                                                                                                                                                      |                                            |
|            | **(Set QInfo)**                                                                                                                                      |                                            |
|            +------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------+
|            | while((bDRAM0_0(0x0e)\<0x02));                                                                                                                       | // writable Count for SQ                   |
|            |                                                                                                                                                      |                                            |
|            | // put command setting to SQ0                                                                                                                        | // Command (Set Value mode) **// gen CQ**\ |
|            |                                                                                                                                                      | // Source Address: 32B alignment\          |
|            | i = 0x00;\                                                                                                                                           | // Target Address: 32B alignment\          |
|            | dDRAM0_1(0x00+i) = 0x08018001;\                                                                                                                      | // Set Value\                              |
|            | dDRAM0_1(0x04+i) = 0x00000000;\                                                                                                                      | // DMA Byte Count\                         |
|            | dDRAM0_1(0x08+i) = 0x5C0D0000;\                                                                                                                      | // Sector Mask bit map\                    |
|            | dDRAM0_1(0x0C+i) = 0x00000078;\                                                                                                                      | // LBA\                                    |
|            | dDRAM0_1(0x10+i) = 0x00000400;\                                                                                                                      | // Reserved\                               |
|            | dDRAM0_1(0x14+i) = 0xFFFFFFFF;\                                                                                                                      | \                                          |
|            | dDRAM0_1(0x18+i) = 0x00000001;\                                                                                                                      | \                                          |
|            | dDRAM0_1(0x1C+i) = 0x00000000;\                                                                                                                      | // Command (Set Value mode)**// gen CQ**\  |
|            | // put command setting to SQ1\                                                                                                                       | // Source Address: 32B alignment\          |
|            | i = 0x20;\                                                                                                                                           | // Target Address: 32B alignment\          |
|            | dDRAM0_1(0x00+i) = 0x08028001;\                                                                                                                      | // Set Value\                              |
|            | dDRAM0_1(0x04+i) = 0x00000000;\                                                                                                                      | // DMA Byte Count\                         |
|            | dDRAM0_1(0x08+i) = 0x5C0D0800;\                                                                                                                      | // Sector Mask bit map\                    |
|            | dDRAM0_1(0x0C+i) = 0x00000056;\                                                                                                                      | // LBA\                                    |
|            | dDRAM0_1(0x10+i) = 0x00000400;\                                                                                                                      | // Reserved                                |
|            | dDRAM0_1(0x14+i) = 0xFFFFFFFF;\                                                                                                                      |                                            |
|            | dDRAM0_1(0x18+i) = 0x00000002;\                                                                                                                      |                                            |
|            | dDRAM0_1(0x1C+i) = 0x00000000;                                                                                                                       |                                            |
+------------+------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------+
| **Step 3** | **Set SQ Write Pointer Increase Unit**                                                                                                               |                                            |
|            +------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------+
|            | wDRAM0_0(0x0C) = 0x02;                                                                                                                               | // send 2 SQ to DMAC (trigger)             |
+------------+------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------+
| **Step 4** | **if SET_CQ=1 in the command setting, the DMAC HW will put a status information to the CQ, FW can use RD_CNT to check whether DMAC is done or not.** |                                            |
|            +------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------+
|            | while((bDRAM0_0(0x32)\<0x02));                                                                                                                       | // Readable Count for CQ                   |
+------------+------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------+
| **Step 5** | **Getting status information from the CQ, and analyzing result of the command operation.**                                                           |                                            |
+------------+------------------------------------------------------------------------------------------------------------------------------------------------------+--------------------------------------------+
| **Note1**  | Start address of each SQ/CQ need align with 32Byte                                                                                                                                                |
+------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **Note2**  |   ---------------------------------------------------------------------------                                                                                                                     |
|            |   if enable the mask_en function, the maximum data byte count is 16KB.                                                                                                                            |
|            |   ---------------------------------------------------------------------------                                                                                                                     |
|            |   the SCT_MSK zero bit should map with data byte count. for example:                                                                                                                              |
|            |                                                                                                                                                                                                   |
|            |   if SCT_MSK=0xFFFF_0001, the byte count should fill 8KB or larger than 8KB                                                                                                                       |
|            |                                                                                                                                                                                                   |
|            |   if SCT_MSK=0xFFFF_FF10, the byte count should fill 4KB or larger than 4KB                                                                                                                       |
|            |                                                                                                                                                                                                   |
|            |   \*Mask only support in DBUF and DRAM Z3                                                                                                                                                         |
|            |   ---------------------------------------------------------------------------                                                                                                                     |
+------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

# SS Design Spec (QBody)

## OP Code( 1 Unit)

+-----------------------------------------------------------------------------------------------------------------------------------------------+
| SQ                                                                                                                                            |
+===============+===============+===============+===============+===============+===============+===============+===============+===============+
| function      | OP0 (4B)      | OP1 (4B)      | OP2 (4B)      | OP3 (4B)      | OP4 (4B)      | OP5 (4B)      | OP6 (4B)      | OP7 (4B)      |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| MSV           | CMD           | SADD0         | LEN           | **X**         | PAT0_L        | PAT0_H        | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| INC           | CMD           | SADD          | LEN           | **X**         | PAT0_L        | PAT0_H        | bMSK0_L       | bMSK0_H       |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| COPY          | CMD           | SADD          | LEN           | TADDR0        | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SEQU          | CMD           | SADD0         | LEN           | bMSK0         | PAT0          | PAT1          | PAT2          | PAT3          |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SNEQ          | CMD           | SADD0         | LEN           | **X**         | PAT0_L        | PAT0_H        | bMSK0_L       | bMSK0_H       |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SMORE         | CMD           | SADD0         | LEN           | **X**         | PAT0_L        | PAT0_H        | bMSK0_L       | bMSK0_H       |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SLESS         | CMD           | SADD0         | LEN           | **X**         | PAT0_L        | PAT0_H        | bMSK0_L       | bMSK0_H       |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SRANG         | CMD           | SADD0         | LEN           | bMSK0         | PAT0          | PAT1          | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SMAX/SMIN     | CMD           | SADD0         | LEN           | **X**         | PAT0_L        | PAT0_H        | bMSK0_L       | bMSK0_H       |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CHKSUM        | CMD           | SADD0         | LEN           | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CRC           | CMD           | SADD0         | LEN           | **X**         | PAT0_L        | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CONE/CZERO    | CMD           | SADD0         | LEN           | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CEQU          | CMD           | SADD0         | LEN           | **X**         | PAT0_L        | PAT0_H        | bMSK0_L       | bMSK0_H       |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CNEQ          | CMD           | SADD0         | LEN           | **X**         | PAT0_L        | PAT0_H        | bMSK0_L       | bMSK0_H       |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CMORE         | CMD           | SADD0         | LEN           | **X**         | PAT0_L        | PAT0_H        | bMSK0_L       | bMSK0_H       |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CLESS         | CMD           | SADD0         | LEN           | **X**         | PAT0_L        | PAT0_H        | bMSK0_L       | bMSK0_H       |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CRANG         | CMD           | SADD0         | LEN           | bMSK0         | PAT0          | PAT1          | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SV            | CMD           | SADD0         | LEN           | **X**         | PAT0_L        | PAT0_H        | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| RDATA         | CMD           | SADD0         | SADD1         | SADD2         | SADD3         | SADD4         | SADD5         | SADD6         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| BLOOM         | CMD           | SADD0         | LEN           | BF_DATA       | XOR           | MULTI         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+

+-----------------------------------------------------------------------------------------------------------------------------------------------+
| 1\. SEQU 8 byte FindMoreTarget (target number = 1)                                                                                            |
+===============+===============+===============+===============+===============+===============+===============+===============+===============+
| SEQU          | CMD           | SADD0         | LEN           | **X**         | PAT0_L        | PAT0_H        | bMSK0_L       | bMSK0_H       |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+

+-----------------------------------------------------------------------------------------------------------------------------------------------+
| CQ                                                                                                                                            |
+===============+===============+===============+===============+===============+===============+===============+===============+===============+
| functin       | OP0 (4B)      | OP1 (4B)      | OP2 (4B)      | OP3 (4B)      | OP4 (4B)      | OP5 (4B)      | OP6 (4B)      | OP7(4B)       |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| MSV           | Status        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| INC           | Status        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| COPY          | Status        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SEQU          | Status        | INDEX0        | INDEX1        | INDEX2        | INDEX3        | INDEX4        | INDEX5        | ADDR          |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SNEQ          | Status        | INDEX0        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SMORE         | Status        | INDEX0        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SLESS         | Status        | INDEX0        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SRANG         | Status        | INDEX0        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SMAX/SMIN     | Status        | INDEX0        | VAL0_L        | VAL0_H        | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CHKSUM        | Status        | CKSUM         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CRC           | Status        | VAL0          | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CONE/CZERO    | Status        | VAL0          | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CEQU          | Status        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CNEQ          | Status        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CMORE         | Status        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CLESS         | Status        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| CRANG         | Status        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SV            | Status        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| RDATA         | Status        | VAL0          | VAL1          | VAL2          | VAL3          | VAL4          | VAL5          | VAL6          |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| BLOOM         | HIT           | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+

+-----------------------------------------------------------------------------------------------------------------------------------------------+
| 1\. SEQU 8 byte FindMoreTarget (target number = 1)                                                                                            |
+===============+===============+===============+===============+===============+===============+===============+===============+===============+
| SEQU          | Status        | INDEX0        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+

## OP Code( 2 Unit)

+-----------------------------------------------------------------------------------------------------------------------------------------------+
| SQ                                                                                                                                            |
+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+
| 1\. SEQU 8 byte and SNEQ/SMORE/SLESS 1/2/4/8 byte continue search with 14 results.                                                            |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SEQU          | CMD           | SADD0         | LEN           | **X**         | PAT0_L        | PAT0_H        | bMSK0_L       | bMSK0_H       |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 2\. SEQU 1/2/4 byte continue search with 14 results                                                                                           |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SEQU          | CMD           | SADD0         | LEN           | bMSK0         | PAT0          | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 3\. SRANG 1/2/4 byte continue search with 14 results                                                                                          |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SRANG         | CMD           | SADD0         | LEN           | bMSK0         | PAT0          | PAT1          | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 4\. SEQU 8 byte FindMoreTarget (2\<=target number \<= 4)                                                                                      |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SEQU          | CMD           | SADD0         | LEN           | **X**         | PAT0_L        | PAT0_H        | bMSK0_L       | bMSK0_H       |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | PAT1_L        | PAT1_H        | PAT2_L        | PAT2_H        | PAT3_L        | PAT3_H        | 　            | 　            |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 5\. SEQU 1/2/4 byte FindMoreTarget (5\<=target number \<= 8)                                                                                  |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SEQU          | CMD           | SADD0         | LEN           | bMSK0         | PAT0          | PAT1          | PAT2          | PAT3          |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | PAT4          | PAT5          | PAT6          | PAT7          | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 6.SHSUM (1\<=target number \<=7)                                                                                                              |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SHSUM         | CMD           | SADDR0        | LEN           | bMSK0_L       | PAT0_L        | PAT1_L        | PAT2_L        | PAT3_L        |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | PAT4_L        | PAT5_L        | PAT6_L        | PAT7_L        | bMSK0_L       | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 7.SMAX/SMIN & NEQ 8 Byte                                                                                                                      |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SMAX          | CMD           | SADDR0        | LEN           | **X**         | PAT0_L        | PAT0_H        | bMSK0_L       | bMSK0_H       |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | bMSK1_L       | bMSK1_H       | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+

+-----------------------------------------------------------------------------------------------------------------------------------------------+
| CQ                                                                                                                                            |
+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+
| 1\. SEQU 8 byte and SNEQ/SMORE/SLESS 1/2/4/8 byte continue search with 14 results.                                                            |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SEQU          | Status        | INDEX0        | INDEX1        | INDEX2        | INDEX3        | INDEX4        | INDEX5        | INDEX6        |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | INDEX7        | INDEX8        | INDEX9        | INDEX10       | INDEX11       | INDEX12       | INDEX13       | ADDR          |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 2\. SEQU 1/2/4 byte continue search with 14 results                                                                                           |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SEQU          | Status        | INDEX0        | INDEX1        | INDEX2        | INDEX3        | INDEX4        | INDEX5        | INDEX6        |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | INDEX7        | INDEX8        | INDEX9        | INDEX10       | INDEX11       | INDEX12       | INDEX13       | ADDR          |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 3\. SRANG 1/2/4 byte continue search with 14 results                                                                                          |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SRANG         | Status        | INDEX0        | INDEX1        | INDEX2        | INDEX3        | INDEX4        | INDEX5        | INDEX6        |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | INDEX7        | INDEX8        | INDEX9        | INDEX10       | INDEX11       | INDEX12       | INDEX13       | ADDR          |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 4\. SEQU 8 byte FindMoreTarget (2\<=target number \<= 4)                                                                                      |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SEQU          | Status        | INDEX0        | INDEX1        | INDEX2        | INDEX3        | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 5\. SEQU 1/2/4 byte FindMoreTarget (5\<=target number \<= 8)                                                                                  |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SEQU          | Status        | INDEX0        | INDEX1        | INDEX2        | INDEX3        | INDEX4        | INDEX5        | INDEX6        |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | INDEX7        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 6.SHSUM (1\<=target number \<=7)                                                                                                              |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SHSUM         | Status        | **X**         | INDEX0        | VAL0          | INDEX1        | VAL1          | INDEX2        | VAL2          |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | INDEX3        | VAL3          | INDEX4        | VAL4          | INDEX5        | VAL5          | INDEX6        | VAL6          |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 7.SMAX/SMIN & NEQ 8 Byte                                                                                                                      |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SMAX          | Status        | RESULT        | VAL_L         | VAL_H         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+

## OP Code( 3 Unit)

+-----------------------------------------------------------------------------------------------------------------------------------------------+
| SQ                                                                                                                                            |
+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+
| 1\. SEQU 8 byte FindMoreTarget (target number = 8)                                                                                            |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SEQU          | CMD           | SADD0         | LEN           | **X**         | PAT0_L        | PAT0_H        | bMSK0_L       | bMSK0_H       |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | PAT1_L        | PAT1_H        | PAT2_L        | PAT2_H        | PAT3_L        | PAT3_H        | PAT4_L        | PAT4_H        |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | PAT5_L        | PAT5_H        | PAT6_L        | PAT6_H        | PAT7_L        | PAT7_H        | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 2 SHSUM (target number = 8)                                                                                                                   |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SHSUM         | CMD           | SADDR0        | LEN           | bMSK0_L       | PAT0_L        | PAT1_L        | PAT2_L        | PAT3_L        |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | PAT4_L        | PAT5_L        | PAT6_L        | PAT7_L        | bMSK0_L       | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 3NMIN                                                                                                                                         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| NMIN          | CMD           | SADD0         | LEN           | **X**         | **X**         | **X**         | bMSK0_L       | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+

+-----------------------------------------------------------------------------------------------------------------------------------------------+
| CQ                                                                                                                                            |
+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+:=============:+
| 1\. SEQU 8 byte FindMoreTarget (target number = 8)                                                                                            |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SEQU          | Status        | INDEX0        | INDEX1        | INDEX2        | INDEX3        | INDEX4        | INDEX5        | INDEX6        |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | INDEX7        | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 2 SHSUM (target number = 8)                                                                                                                   |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| SHSUM         | Status        | **X**         | INDEX0        | VAL0          | INDEX1        | VAL1          | INDEX2        | VAL2          |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | INDEX3        | VAL3          | INDEX4        | VAL4          | INDEX5        | VAL5          | INDEX6        | VAL6          |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | INDEX7        | VAL7          | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| 3NMIN                                                                                                                                         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
| NMIN          | Status        | **X**         | RESULT0       | VAL0          | RESULT1       | VAL1          | RESULT2       | VAL2          |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | RESULT3       | VAL3          | RESULT4       | VAL4          | RESULT5       | VAL5          | RESULT6       | VAL6          |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+
|               | RESULT7       | VAL7          | **X**         | **X**         | **X**         | **X**         | **X**         | **X**         |
+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+---------------+

## OP Discription For Each Command

### MSV mode

+----------------+----------------+----------------+-----------------------+
| MSV            | Bit Map        | Bit Symbol     | Description           |
+================+================+================+=======================+
| CMD            | 5 \~ 0         | OP             | OP Code (6\'b000000)  |
|                +----------------+----------------+-----------------------+
|                | 7 \~ 6         | CMD_UNIT       | 2\'b01: 1 Unit.       |
|                +----------------+----------------+-----------------------+
|                | 9 \~ 8         | SIZ_UNIT       | 2\'h0: 1B\            |
|                |                |                | 2\'h1: 2B\            |
|                |                |                | 2\'h2: 4B\            |
|                |                |                | 2\'h3: 8B\            |
|                |                |                | Others: reserve       |
|                +----------------+----------------+-----------------------+
|                | 10             | MSK_EN         | Bit Mask Enable       |
|                +----------------+----------------+-----------------------+
|                | 31\~11         | Dep            | Depend On FW          |
+----------------+----------------+----------------+-----------------------+
| SADDR0         | 31\~0          | SADDR0         | Source Address        |
+----------------+----------------+----------------+-----------------------+
| LEN            | 31\~0          | LEN            | DMA Byte Count        |
+----------------+----------------+----------------+-----------------------+
| Dep            | 31\~0          | Dep            | Depend On FW          |
+----------------+----------------+----------------+-----------------------+
| PAT0_L         | 31\~0          | PAT0_L         | Pattern 0 Low 4 Byte  |
+----------------+----------------+----------------+-----------------------+
| PAT0_H         | 31\~0          | PAT0_H         | Pattern 0 High 4 Byte |
+----------------+----------------+----------------+-----------------------+
| Dep            | 31\~0          | Dep            | Depend On FW          |
+----------------+----------------+----------------+-----------------------+
| Dep            | 31\~0          | Dep            | Depend On FW          |
+----------------+----------------+----------------+-----------------------+
| CQ                                                                       |
+----------------+----------------+----------------+-----------------------+
| STA            | 31\~0          | STA            | Status                |
|                +----------------+----------------+-----------------------+
|                | 31\~0          | RSV.           | Reserved              |
|                +----------------+----------------+-----------------------+
|                | 31\~0          | RSV.           | Reserved              |
|                +----------------+----------------+-----------------------+
|                | 31\~0          | RSV.           | Reserved              |
|                +----------------+----------------+-----------------------+
|                | 31\~0          | RSV.           | Reserved              |
|                +----------------+----------------+-----------------------+
|                | 31\~0          | RSV.           | Reserved              |
|                +----------------+----------------+-----------------------+
|                | 31\~0          | RSV.           | Reserved              |
|                +----------------+----------------+-----------------------+
|                | 31\~0          | RSV.           | Reserved              |
+----------------+----------------+----------------+-----------------------+

**Note**: MSK_EN must be set to 1.

### INC mode

+----------------+----------------+----------------+------------------------+
| INC            | Bit Map        | Bit Symbol     | Description            |
+================+================+================+========================+
| CMD            | 5 \~ 0         | OP             | OP Code (6\'b000001)   |
|                +----------------+----------------+------------------------+
|                | 7 \~ 6         | CMD_UNIT       | 2\'b01: 1 Unit.        |
|                +----------------+----------------+------------------------+
|                | 9 \~ 8         | SIZ_UNIT       | 2\'h0: 1B\             |
|                |                |                | 2\'h1: 2B\             |
|                |                |                | 2\'h2: 4B\             |
|                |                |                | 2\'h3: 8B\             |
|                |                |                | Others: reserve        |
|                +----------------+----------------+------------------------+
|                | 10             | MSK_EN         | Bit Mask Enable        |
|                +----------------+----------------+------------------------+
|                | 31\~11         | Dep            | Depend On FW           |
+----------------+----------------+----------------+------------------------+
| SADDR0         | 31\~0          | SADDR0         | Source Address         |
+----------------+----------------+----------------+------------------------+
| LEN            | 31\~0          | LEN            | DMA Byte Count         |
+----------------+----------------+----------------+------------------------+
| Dep            | 31\~0          | Dep            | Depend On FW           |
+----------------+----------------+----------------+------------------------+
| PAT0_L         | 31\~0          | PAT0_L         | Pattern 0 Low 4 Byte   |
+----------------+----------------+----------------+------------------------+
| PAT0_H         | 31\~0          | PAT0_H         | Pattern 0 High 4 Byte  |
+----------------+----------------+----------------+------------------------+
| bMSK0_L        | 31\~0          | bMSK0_L        | Bit Mask 0 Low 4 Byte  |
+----------------+----------------+----------------+------------------------+
| bMSK0_H        | 31\~0          | bMSK0_H        | Bit Mask 0 High 4 Byte |
+----------------+----------------+----------------+------------------------+
| CQ                                                                        |
+----------------+----------------+----------------+------------------------+
| STA            | 31\~0          | STA            | Status                 |
|                +----------------+----------------+------------------------+
|                | 31\~0          | RSV.           | Reserved               |
|                +----------------+----------------+------------------------+
|                | 31\~0          | RSV.           | Reserved               |
|                +----------------+----------------+------------------------+
|                | 31\~0          | RSV.           | Reserved               |
|                +----------------+----------------+------------------------+
|                | 31\~0          | RSV.           | Reserved               |
|                +----------------+----------------+------------------------+
|                | 31\~0          | RSV.           | Reserved               |
|                +----------------+----------------+------------------------+
|                | 31\~0          | RSV.           | Reserved               |
|                +----------------+----------------+------------------------+
|                | 31\~0          | RSV.           | Reserved               |
+----------------+----------------+----------------+------------------------+

### COPY mode

+----------------+----------------+----------------+-----------------------+
| COPY           | Bit Map        | Bit Symbol     | Description           |
+================+================+================+=======================+
| CMD            | 5 \~ 0         | OP             | OP Code (6\'b000010)  |
|                +----------------+----------------+-----------------------+
|                | 7 \~ 6         | CMD_UNIT       | 2\'b01: 1 Unit.       |
|                +----------------+----------------+-----------------------+
|                | 9 \~ 8         | Dep            | Depend On FW          |
|                +----------------+----------------+-----------------------+
|                | 10             | Dep            | Depend On FW          |
|                +----------------+----------------+-----------------------+
|                | 31\~11         | Dep            | Depend On FW          |
+----------------+----------------+----------------+-----------------------+
| SADDR0         | 31\~0          | SADDR0         | Source Address        |
+----------------+----------------+----------------+-----------------------+
| LEN            | 31\~0          | LEN            | DMA Byte Count        |
+----------------+----------------+----------------+-----------------------+
| TADDR0         | 31\~0          | TADDR          | Target Address        |
+----------------+----------------+----------------+-----------------------+
| Dep            | 31\~0          | PAT0_L         | Depend On FW          |
+----------------+----------------+----------------+-----------------------+
| Dep            | 31\~0          | PAT0_H         | Depend On FW          |
+----------------+----------------+----------------+-----------------------+
| Dep            | 31\~0          | Dep            | Depend On FW          |
+----------------+----------------+----------------+-----------------------+
| Dep            | 31\~0          | Dep            | Depend On FW          |
+----------------+----------------+----------------+-----------------------+
| CQ                                                                       |
+----------------+----------------+----------------+-----------------------+
| STA            | 31\~0          | STA            | Status                |
|                +----------------+----------------+-----------------------+
|                | 31\~0          | RSV.           | Reserved              |
|                +----------------+----------------+-----------------------+
|                | 31\~0          | RSV.           | Reserved              |
|                +----------------+----------------+-----------------------+
|                | 31\~0          | RSV.           | Reserved              |
|                +----------------+----------------+-----------------------+
|                | 31\~0          | RSV.           | Reserved              |
|                +----------------+----------------+-----------------------+
|                | 31\~0          | RSV.           | Reserved              |
|                +----------------+----------------+-----------------------+
|                | 31\~0          | RSV.           | Reserved              |
|                +----------------+----------------+-----------------------+
|                | 31\~0          | RSV.           | Reserved              |
+----------------+----------------+----------------+-----------------------+

### SEQU mode

+----------------+----------------+----------------+----------------------------+
| SEQU           | Bit Map        | Bit Symbol     | Description                |
+================+================+================+============================+
| CMD            | 5 \~ 0         | OP             | OP Code (6\'b000011)       |
|                +----------------+----------------+----------------------------+
|                | 7 \~ 6         | CMD_UNIT       | 2\'b01: 1 Unit.            |
|                +----------------+----------------+----------------------------+
|                | 9 \~ 8         | SIZ_UNIT       | 2\'h0: 1B\                 |
|                |                |                | 2\'h1: 2B\                 |
|                |                |                | 2\'h2: 4B\                 |
|                |                |                | 2\'h3: 8B\                 |
|                |                |                | Others: reserve            |
|                +----------------+----------------+----------------------------+
|                | 10             | MSK_EN         | Bit Mask Enable            |
|                +----------------+----------------+----------------------------+
|                | 11             | SHB_EN         | Depend On FW               |
|                +----------------+----------------+----------------------------+
|                | 12             | FIND_MORE      | Find More Enable(找到就停) |
|                +----------------+----------------+----------------------------+
|                | 15\~13         | Target         | One-Eight Target (0\~7)    |
|                +----------------+----------------+----------------------------+
|                | 16             | Number         | 1\'b0/1\'b1: 6/14 amout    |
|                +----------------+----------------+----------------------------+
|                | 23\~17         | Dep            | Depend On FW               |
|                +----------------+----------------+----------------------------+
|                | 31\~24         | Threshold      | Valid From which Result    |
+----------------+----------------+----------------+----------------------------+
| SADDR0         | 31\~0          | SADDR0         | Source Address 0           |
+----------------+----------------+----------------+----------------------------+
| LEN            | 31\~0          | LEN            | DMA Byte Count             |
+----------------+----------------+----------------+----------------------------+
| bMASK0         | 31\~0          | bMASK0         | Bit Mask                   |
+----------------+----------------+----------------+----------------------------+
| PAT0           | 31\~0          | PAT0           | Search Pattern 0           |
+----------------+----------------+----------------+----------------------------+
| PAT1           | 31\~0          | PAT1           | Search Pattern 1           |
+----------------+----------------+----------------+----------------------------+
| PAT2           | 31\~0          | PAT2           | Search Pattern 2           |
+----------------+----------------+----------------+----------------------------+
| PAT3           | 31\~0          | PAT3           | Search Pattern 3           |
+----------------+----------------+----------------+----------------------------+
| CQ                                                                            |
+----------------+----------------+----------------+----------------------------+
| STA            | 7\~0           | STA            | Status                     |
|                +----------------+----------------+----------------------------+
|                | 8              | Match\[0\]     | Match or not               |
|                +----------------+----------------+----------------------------+
|                | 9              | SH_NOT_DONE    | Search not Done            |
|                |                +----------------+----------------------------+
|                |                | Match\[1\]     | Match or not (FMR_EN)      |
|                +----------------+----------------+----------------------------+
|                | 10             | Match\[2\]     | Match or not (FMR_EN)      |
|                +----------------+----------------+----------------------------+
|                | 11             | Match\[3\]     | Match or not (FMR_EN)      |
|                +----------------+----------------+----------------------------+
|                | 12             | Match\[4\]     | Match or not (FMR_EN)      |
|                +----------------+----------------+----------------------------+
|                | 13             | Match\[5\]     | Match or not (FMR_EN)      |
|                +----------------+----------------+----------------------------+
|                | 14             | Match\[6\]     | Match or not (FMR_EN)      |
|                +----------------+----------------+----------------------------+
|                | 15             | Match\[7\]     | Match or not (FMR_EN)      |
|                +----------------+----------------+----------------------------+
|                | 31\~16         | Match_CNT      | Match Count                |
|                +----------------+----------------+----------------------------+
|                | 31 \~ 0        | SH_RESULT_0    | Search Result 0            |
|                +----------------+----------------+----------------------------+
|                | 31 \~ 0        | SH_RESULT_1    | Search Result 1            |
|                +----------------+----------------+----------------------------+
|                | 31 \~ 0        | SH_RESULT_2    | Search Result 2            |
|                +----------------+----------------+----------------------------+
|                | 31 \~ 0        | SH_RESULT_3    | Search Result 3            |
|                +----------------+----------------+----------------------------+
|                | 31 \~ 0        | SH_RESULT_4    | Search Result 4            |
|                +----------------+----------------+----------------------------+
|                | 31 \~ 0        | SH_RESULT_5    | Search Result 5            |
|                +----------------+----------------+----------------------------+
|                | 31 \~ 0        | NXT_SH_ADDR    | Next Search Addr           |
+----------------+----------------+----------------+----------------------------+

**Table : Command Unit of SEQU(Find More Target):**

+-----------------+------+------+------+------+
| OPSize          | 1    | 2    | 4    | 8    |
|                 |      |      |      |      |
| Target Num      |      |      |      |      |
+:===============:+:====:+:====:+:====:+:====:+
| 1               | 1    | 1    | 1    | 1    |
+-----------------+------+------+------+------+
| 2               | 1    | 1    | 1    | 2    |
+-----------------+------+------+------+------+
| 3               | 1    | 1    | 1    | 2    |
+-----------------+------+------+------+------+
| 4               | 1    | 1    | 1    | 2    |
+-----------------+------+------+------+------+
| 5               | 2    | 2    | 2    | 3    |
+-----------------+------+------+------+------+
| 6               | 2    | 2    | 2    | 3    |
+-----------------+------+------+------+------+
| 7               | 2    | 2    | 2    | 3    |
+-----------------+------+------+------+------+
| 8               | 2    | 2    | 2    | 3    |
+-----------------+------+------+------+------+

**Note1**: In single search (FIND_MORE=0), SS will return a number of search results, and return the next search address if SH_NOT_DONE=1, for example, when NUMBER=0 (single search 6 results), and the data is "A,B,C,A,D,E,A,G,A,B,A,D,A,H", and you want to find pattern = "A", SS will return search result\[0:5\]=\[0,3,6,8,10,12\], and return next search result = 13;

**Note2**: In find more search mode (FIND_MORE=1), SS will return the first match result of each search pattern, for example, when FIND_MORE=1, and the data is "A,B,C,A,D,E,A,G,A,B,A,D,A,H", and you want to find 4 pattern = "A,B,C,D", SS will return search result\[0:3\]=\[0,1,2,4\], and return no next search result ;

**Note3**: In SHB_EN=1, SS will search from back and return only one search result, and Threshold has no effect.

**Note4**: Please refer to chapter 3.2 and 3.3 to get SQ/CQ format with Unit\>=2.

### SNEQ mode

+-----------------+-----------------+-----------------+-------------------------+
| SNEQ            | Bit Map         | Bit Symbol      | Description             |
+=================+=================+=================+=========================+
| CMD             | 5 \~ 0          | OP              | OP Code (6\'b000100)    |
|                 +-----------------+-----------------+-------------------------+
|                 | 7 \~ 6          | CMD_UNIT        | 2\'b01: 1 Unit.         |
|                 +-----------------+-----------------+-------------------------+
|                 | 9 \~ 8          | SIZ_UNIT        | 2\'h0: 1B\              |
|                 |                 |                 | 2\'h1: 2B\              |
|                 |                 |                 | 2\'h2: 4B\              |
|                 |                 |                 | 2\'h3: 8B\              |
|                 |                 |                 | Others: reserve         |
|                 +-----------------+-----------------+-------------------------+
|                 | 10              | MSK_EN          | Bit Mask Enable         |
|                 +-----------------+-----------------+-------------------------+
|                 | 11              | SHB_EN          | Search Back Enable      |
|                 +-----------------+-----------------+-------------------------+
|                 | 15 \~12         | Dep             | Depend On FW            |
|                 +-----------------+-----------------+-------------------------+
|                 | 16              | Number          | 1\'b0/1\'b1: 6/14 amout |
|                 +-----------------+-----------------+-------------------------+
|                 | 23\~17          | Dep             | Depend On FW            |
|                 +-----------------+-----------------+-------------------------+
|                 | 31\~24          | Threshold       | Valid From which Result |
+-----------------+-----------------+-----------------+-------------------------+
| SADDR0          | 31\~0           | SADDR0          | Source Address          |
+-----------------+-----------------+-----------------+-------------------------+
| LEN             | 31\~0           | LEN             | DMA Byte Count          |
+-----------------+-----------------+-----------------+-------------------------+
| Dep             | 31\~0           | Dep             | Depend On FW            |
+-----------------+-----------------+-----------------+-------------------------+
| PAT0_L          | 31\~0           | PAT0_L          | Pattern 0 Low 4 Byte    |
+-----------------+-----------------+-----------------+-------------------------+
| PAT0_H          | 31\~0           | PAT0            | Pattern 0 High 4 Byte   |
+-----------------+-----------------+-----------------+-------------------------+
| bMSK0_L         | 31\~0           | bMSK0_L         | Bit Mask 0 Low 4 Byte   |
+-----------------+-----------------+-----------------+-------------------------+
| bMSK0_H         | 31\~0           | bMSK0_H         | Bit Mask 0 High 4 Byte  |
+-----------------+-----------------+-----------------+-------------------------+
| CQ                                                                            |
+-----------------+-----------------+-----------------+-------------------------+
| STA             | 7\~0            | STA             | Status                  |
|                 +-----------------+-----------------+-------------------------+
|                 | 8               | Match\[0\]      | Match or not            |
|                 +-----------------+-----------------+-------------------------+
|                 | 9               | SH_NOT_DONE     | Search not Done         |
|                 +-----------------+-----------------+-------------------------+
|                 | 15\~10          | RSV.            | Reserved                |
|                 +-----------------+-----------------+-------------------------+
|                 | 31\~16          | Match_CNT       | Match Count             |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_0     | Search Result 0         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_1     | Search Result 1         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_2     | Search Result 2         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_3     | Search Result 3         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_4     | Search Result 4         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_5     | Search Result 5         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | NXT_SH_ADDR     | Next Search Addr        |
+-----------------+-----------------+-----------------+-------------------------+

### SMORE mode

+-----------------+-----------------+-----------------+-------------------------+
| SMORE           | Bit Map         | Bit Symbol      | Description             |
+=================+=================+=================+=========================+
| CMD             | 5 \~ 0          | OP              | OP Code (6\'b000101)    |
|                 +-----------------+-----------------+-------------------------+
|                 | 7 \~ 6          | CMD_UNIT        | 2\'b01: 1 Unit.         |
|                 +-----------------+-----------------+-------------------------+
|                 | 9 \~ 8          | SIZ_UNIT        | 2\'h0: 1B\              |
|                 |                 |                 | 2\'h1: 2B\              |
|                 |                 |                 | 2\'h2: 4B\              |
|                 |                 |                 | 2\'h3: 8B\              |
|                 |                 |                 | Others: reserve         |
|                 +-----------------+-----------------+-------------------------+
|                 | 10              | MSK_EN          | Bit Mask Enable         |
|                 +-----------------+-----------------+-------------------------+
|                 | 11              | SHB_EN          | Search Back Enable      |
|                 +-----------------+-----------------+-------------------------+
|                 | 15 \~12         | Dep             | Depend On FW            |
|                 +-----------------+-----------------+-------------------------+
|                 | 16              | Number          | 1\'b0/1\'b1: 6/14 amout |
|                 +-----------------+-----------------+-------------------------+
|                 | 23\~17          | Dep             | Depend On FW            |
|                 +-----------------+-----------------+-------------------------+
|                 | 31\~24          | Threshold       | Valid From which Result |
+-----------------+-----------------+-----------------+-------------------------+
| SADDR0          | 31\~0           | SADDR0          | Source Address          |
+-----------------+-----------------+-----------------+-------------------------+
| LEN             | 31\~0           | LEN             | DMA Byte Count          |
+-----------------+-----------------+-----------------+-------------------------+
| Dep             | 31\~0           | Dep             | Depend On FW            |
+-----------------+-----------------+-----------------+-------------------------+
| PAT0_L          | 31\~0           | PAT0_L          | Pattern 0 Low 4 Byte    |
+-----------------+-----------------+-----------------+-------------------------+
| PAT0_H          | 31\~0           | PAT0            | Pattern 0 High 4 Byte   |
+-----------------+-----------------+-----------------+-------------------------+
| bMSK0_L         | 31\~0           | bMSK0_L         | Bit Mask 0 Low 4 Byte   |
+-----------------+-----------------+-----------------+-------------------------+
| bMSK0_H         | 31\~0           | bMSK0_H         | Bit Mask 0 High 4 Byte  |
+-----------------+-----------------+-----------------+-------------------------+
| CQ                                                                            |
+-----------------+-----------------+-----------------+-------------------------+
| STA             | 7\~0            | RSV.            | Reserved                |
|                 +-----------------+-----------------+-------------------------+
|                 | 8               | Match\[0\]      | Match or not            |
|                 +-----------------+-----------------+-------------------------+
|                 | 9               | SH_NOT_DONE     | Search not Done         |
|                 +-----------------+-----------------+-------------------------+
|                 | 15\~10          | RSV.            | Reserved                |
|                 +-----------------+-----------------+-------------------------+
|                 | 31\~16          | Match_CNT       | Match Count             |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_0     | Search Result 0         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_1     | Search Result 1         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_2     | Search Result 2         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_3     | Search Result 3         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_4     | Search Result 4         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_5     | Search Result 5         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | NXT_SH_ADDR     | Next Search Addr        |
+-----------------+-----------------+-----------------+-------------------------+

### SLESS mode

+-----------------+-----------------+-----------------+-------------------------+
| SLESS           | Bit Map         | Bit Symbol      | Description             |
+=================+=================+=================+=========================+
| CMD             | 5 \~ 0          | OP              | OP Code (6\'b000110)    |
|                 +-----------------+-----------------+-------------------------+
|                 | 7 \~ 6          | CMD_UNIT        | 2\'b01: 1 Unit.         |
|                 +-----------------+-----------------+-------------------------+
|                 | 9 \~ 8          | SIZ_UNIT        | 2\'h0: 1B\              |
|                 |                 |                 | 2\'h1: 2B\              |
|                 |                 |                 | 2\'h2: 4B\              |
|                 |                 |                 | 2\'h3: 8B\              |
|                 |                 |                 | Others: reserve         |
|                 +-----------------+-----------------+-------------------------+
|                 | 10              | MSK_EN          | Bit Mask Enable         |
|                 +-----------------+-----------------+-------------------------+
|                 | 11              | SHB_EN          | Search Back Enable      |
|                 +-----------------+-----------------+-------------------------+
|                 | 15 \~12         | Dep             | Depend On FW            |
|                 +-----------------+-----------------+-------------------------+
|                 | 16              | Number          | 1\'b0/1\'b1: 6/14 amout |
|                 +-----------------+-----------------+-------------------------+
|                 | 23\~17          | Dep             | Depend On FW            |
|                 +-----------------+-----------------+-------------------------+
|                 | 31\~24          | Threshold       | Valid From which Result |
+-----------------+-----------------+-----------------+-------------------------+
| SADDR0          | 31\~0           | SADDR0          | Source Address          |
+-----------------+-----------------+-----------------+-------------------------+
| LEN             | 31\~0           | LEN             | DMA Byte Count          |
+-----------------+-----------------+-----------------+-------------------------+
| Dep             | 31\~0           | Dep             | Depend On FW            |
+-----------------+-----------------+-----------------+-------------------------+
| PAT0_L          | 31\~0           | PAT0_L          | Pattern 0 Low 4 Byte    |
+-----------------+-----------------+-----------------+-------------------------+
| PAT0_H          | 31\~0           | PAT0            | Pattern 0 High 4 Byte   |
+-----------------+-----------------+-----------------+-------------------------+
| bMSK0_L         | 31\~0           | bMSK0_L         | Bit Mask 0 Low 4 Byte   |
+-----------------+-----------------+-----------------+-------------------------+
| bMSK0_H         | 31\~0           | bMSK0_H         | Bit Mask 0 High 4 Byte  |
+-----------------+-----------------+-----------------+-------------------------+
| CQ                                                                            |
+-----------------+-----------------+-----------------+-------------------------+
| STA             | 7\~0            | RSV.            | Reserved                |
|                 +-----------------+-----------------+-------------------------+
|                 | 8               | Match\[0\]      | Match or not            |
|                 +-----------------+-----------------+-------------------------+
|                 | 9               | SH_NOT_DONE     | Search not Done         |
|                 +-----------------+-----------------+-------------------------+
|                 | 15\~10          | RSV.            | Reserved                |
|                 +-----------------+-----------------+-------------------------+
|                 | 31\~16          | Match_CNT       | Match Count             |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_0     | Search Result 0         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_1     | Search Result 1         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_2     | Search Result 2         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_3     | Search Result 3         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_4     | Search Result 4         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_5     | Search Result 5         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | NXT_SH_ADDR     | Next Search Addr        |
+-----------------+-----------------+-----------------+-------------------------+

### SRANG mode

+-----------------+-----------------+-----------------+-------------------------+
| SRANG           | Bit Map         | Bit Symbol      | Description             |
+=================+=================+=================+=========================+
| CMD             | 5 \~ 0          | OP              | OP Code (6\'b000111)    |
|                 +-----------------+-----------------+-------------------------+
|                 | 7 \~ 6          | CMD_UNIT        | 2\'b01: 1 Unit.         |
|                 +-----------------+-----------------+-------------------------+
|                 | 9 \~ 8          | SIZ_UNIT        | 2\'h0: 1B\              |
|                 |                 |                 | 2\'h1: 2B\              |
|                 |                 |                 | 2\'h2: 4B\              |
|                 |                 |                 | Others: reserve         |
|                 +-----------------+-----------------+-------------------------+
|                 | 10              | MSK_EN          | Bit Mask Enable         |
|                 +-----------------+-----------------+-------------------------+
|                 | 11              | SHB_EN          | Search Back Enable      |
|                 +-----------------+-----------------+-------------------------+
|                 | 15 \~12         | Dep             | Depend On FW            |
|                 +-----------------+-----------------+-------------------------+
|                 | 16              | Number          | 1\'b0/1\'b1: 6/14 amout |
|                 +-----------------+-----------------+-------------------------+
|                 | 23\~17          | Dep             | Depend On FW            |
|                 +-----------------+-----------------+-------------------------+
|                 | 31\~24          | Threshold       | Valid From which Result |
+-----------------+-----------------+-----------------+-------------------------+
| SADDR0          | 31\~0           | SADDR0          | Source Address          |
+-----------------+-----------------+-----------------+-------------------------+
| LEN             | 31\~0           | LEN             | DMA Byte Count          |
+-----------------+-----------------+-----------------+-------------------------+
| bMASK0          | 31\~0           | bMASK0          | Bit Mask                |
+-----------------+-----------------+-----------------+-------------------------+
| PAT0            | 31\~0           | PAT0            | Search Pattern 0        |
+-----------------+-----------------+-----------------+-------------------------+
| PAT1            | 31\~0           | PAT1            | Search Pattern 1        |
+-----------------+-----------------+-----------------+-------------------------+
| Dep             | 31\~0           | Dep             | Depend On FW            |
+-----------------+-----------------+-----------------+-------------------------+
| Dep             | 31\~0           | Dep             | Depend On FW            |
+-----------------+-----------------+-----------------+-------------------------+
| CQ                                                                            |
+-----------------+-----------------+-----------------+-------------------------+
| STA             | 7\~0            | RSV.            | Reserved                |
|                 +-----------------+-----------------+-------------------------+
|                 | 8               | Match\[0\]      | Match or not            |
|                 +-----------------+-----------------+-------------------------+
|                 | 9               | SH_NOT_DONE     | Search not Done         |
|                 +-----------------+-----------------+-------------------------+
|                 | 15\~10          | RSV.            | Reserved                |
|                 +-----------------+-----------------+-------------------------+
|                 | 31\~16          | Match_CNT       | Match Count             |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_0     | Search Result 0         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_1     | Search Result 1         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_2     | Search Result 2         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_3     | Search Result 3         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_4     | Search Result 4         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | SH_RESULT_5     | Search Result 5         |
|                 +-----------------+-----------------+-------------------------+
|                 | 31 \~ 0         | NXT_SH_ADDR     | Next Search Addr        |
+-----------------+-----------------+-----------------+-------------------------+

### SMAX/SMIN mode

+------------------+------------------+------------------+------------------------+
| SMax/SMin        | Bit Map          | Bit Symbol       | Description            |
+==================+==================+==================+========================+
| CMD              | 5 \~ 0           | OP               | OP Code (6\'b001000)   |
|                  +------------------+------------------+------------------------+
|                  | 7 \~ 6           | CMD_UNIT         | 2\'b01: 1 Unit.        |
|                  +------------------+------------------+------------------------+
|                  | 9 \~ 8           | SIZ_UNIT         | 2\'h0: 1B\             |
|                  |                  |                  | 2\'h1: 2B\             |
|                  |                  |                  | 2\'h2: 4B\             |
|                  |                  |                  | 2\'h3: 8B\             |
|                  |                  |                  | Others: reserve        |
|                  +------------------+------------------+------------------------+
|                  | 10               | MSK_EN           | Bit Mask Enable        |
|                  +------------------+------------------+------------------------+
|                  | 11               | IGNORE_EN        | Ignore Enable          |
|                  +------------------+------------------+------------------------+
|                  | 12               | MIN_EN           | Search Minimum Enable  |
|                  +------------------+------------------+------------------------+
|                  | 13               | NEQ_EN           | Not Equal Enable       |
|                  +------------------+------------------+------------------------+
|                  | 31\~14           | Dep              | Depend On FW           |
+------------------+------------------+------------------+------------------------+
| SADDR0           | 31\~0            | SADDR0           | Source Address         |
+------------------+------------------+------------------+------------------------+
| LEN              | 31\~0            | LEN              | DMA Byte Count         |
+------------------+------------------+------------------+------------------------+
| Dep              | 31\~0            | Dep              | Depend On FW           |
+------------------+------------------+------------------+------------------------+
| PAT0_L           | 31\~0            | PAT0_L           | Pattern 0 Low 4 Byte   |
+------------------+------------------+------------------+------------------------+
| PAT0_H           | 31\~0            | PAT0_H           | Pattern 0 High 4 Byte  |
+------------------+------------------+------------------+------------------------+
| bMSK0_L          | 31\~0            | bMSK0_L          | Bit Mask 0 Low 4 Byte  |
+------------------+------------------+------------------+------------------------+
| bMSK0_H          | 31\~0            | bMSK0_H          | Bit Mask 0 High 4 Byte |
+------------------+------------------+------------------+------------------------+
| CQ                                                                              |
+------------------+------------------+------------------+------------------------+
| STA              | 31\~0            | STA              | Status                 |
|                  +------------------+------------------+------------------------+
|                  | 31\~0            | SH_RESULT        | Search Result          |
|                  +------------------+------------------+------------------------+
|                  | 31\~0            | SH_VAL_L         | Search Val Low 4 Byte  |
|                  +------------------+------------------+------------------------+
|                  | 31\~0            | SH_VAL_H         | Search Val High 4 Byte |
|                  +------------------+------------------+------------------------+
|                  | 31\~0            | RSV.             | Reserved               |
|                  +------------------+------------------+------------------------+
|                  | 31\~0            | RSV.             | Reserved               |
|                  +------------------+------------------+------------------------+
|                  | 31\~0            | RSV.             | Reserved               |
|                  +------------------+------------------+------------------------+
|                  | 31\~0            | RSV.             | Reserved               |
+------------------+------------------+------------------+------------------------+

**Note**: In IGNORE_EN=1, SS will ignore the data which pass through bMSK0 not equals to PAT0, and search the max/min of those unignored data which pass through bMSK0.

[NEQ_EN=1 and Operation Size != 8]{.underline}

+----------------+----------------+----------------+-----------------------------+
| SMax/SMin      | Bit Map        | Bit Symbol     | Description                 |
+================+================+================+=============================+
| CMD            | 5 \~ 0         | OP             | OP Code (6\'b001000)        |
|                +----------------+----------------+-----------------------------+
|                | 7 \~ 6         | CMD_UNIT       | 2\'b01: 1 Unit.             |
|                +----------------+----------------+-----------------------------+
|                | 9 \~ 8         | SIZ_UNIT       | 2\'h0: 1B\                  |
|                |                |                | 2\'h1: 2B\                  |
|                |                |                | 2\'h2: 4B\                  |
|                |                |                | Others: reserve             |
|                +----------------+----------------+-----------------------------+
|                | 10             | MSK_EN         | Bit Mask Enable             |
|                +----------------+----------------+-----------------------------+
|                | 11             | IGNORE_EN      | Ignore Enable               |
|                +----------------+----------------+-----------------------------+
|                | 12             | MIN_EN         | Search Minimum Enable       |
|                +----------------+----------------+-----------------------------+
|                | 13             | NEQ_EN         | Not Equal Enable            |
|                +----------------+----------------+-----------------------------+
|                | 31\~13         | Dep            | Depend On FW                |
+----------------+----------------+----------------+-----------------------------+
| SADDR0         | 31\~0          | SADDR0         | Source Address              |
+----------------+----------------+----------------+-----------------------------+
| LEN            | 31\~0          | LEN            | DMA Byte Count              |
+----------------+----------------+----------------+-----------------------------+
| Dep            | 31\~0          | Dep            | Depend On FW                |
+----------------+----------------+----------------+-----------------------------+
| PAT0_L         | 31\~0          | PAT0_L         | Pattern 0 Low 4 Byte        |
+----------------+----------------+----------------+-----------------------------+
| PAT0_H         | 31\~0          | PAT0_H         | Pattern 0 High 4 Byte       |
+----------------+----------------+----------------+-----------------------------+
| bMSK0_L        | 31\~0          | bMSK0_L        | Bit Mask 0 Low 4 Byte (NEQ) |
+----------------+----------------+----------------+-----------------------------+
| bMSK1_L        | 31\~0          | bMSK1_L        | Bit Mask 1 Low 4 Byte       |
+----------------+----------------+----------------+-----------------------------+
| CQ                                                                             |
+----------------+----------------+----------------+-----------------------------+
| STA            | 31\~0          | RSV.           | Reserved                    |
|                +----------------+----------------+-----------------------------+
|                | 31\~0          | SH_RESULT      | Search Result               |
|                +----------------+----------------+-----------------------------+
|                | 31\~0          | SH_VAL_L       | Search Val Low 4 Byte       |
|                +----------------+----------------+-----------------------------+
|                | 31\~0          | SH_VAL_H       | Search Val High 4 Byte      |
|                +----------------+----------------+-----------------------------+
|                | 31\~0          | RSV.           | Reserved                    |
|                +----------------+----------------+-----------------------------+
|                | 31\~0          | RSV.           | Reserved                    |
|                +----------------+----------------+-----------------------------+
|                | 31\~0          | RSV.           | Reserved                    |
|                +----------------+----------------+-----------------------------+
|                | 31\~0          | RSV.           | Reserved                    |
+----------------+----------------+----------------+-----------------------------+

**Note**: In NEQ_EN=1 and Operation size = 8 case, please refer to chapter 3.2.

**Note2**: In NEQ_EN=1, SS will ignore the data which pass through bMSK0 not equals to PAT0, and search the max/min of those unignored data which pass through bMSK1.

**Note3**: One cannot set NEQ_EN=1 and IGNORE_EN=1 simultaneously.

**\**

[Example:]{.underline}

  -----------------------------------------------------------------------
             0x2                        0xA      0xA      0xA      0xA
  -------- -------- -------- -------- -------- -------- -------- --------
             0x7                        0xB      0xB      0xB      0xB

             0x5                        0xC      0xC      0xC      0xC

             0x4                        0xD      0xD      0xD      0xD

             0x8                        0x0      0x1      0x2      0x3
  -----------------------------------------------------------------------

IGNORE:

Function: find max/min of A & value of A != 0x0123

ADDR_0:

BYTE_COUNT: 20

Size: 4 byte (1/2/4/8)

bMASK0_L : 0x0000_FFFF (area A)

PAT0_L: 0x0000_0123

Return:

MIN_MAX_ADDR : 3

MIN_MAX_VALUE: 0xDDDD

NEQ:

Function: find max/min of B & value of A != 0x0123

ADDR_0:

BYTE_COUNT: 20

Size: 4 byte (1/2/4/8)

bMASK0_L : 0x0000_FFFF (area A)

bMASK1_L: 0x0F00_0000 (area B)

PAT0_L: 0x0000_0123

Return:

MIN_MAX_ADDR : 1

MIN_MAX_VALUE: 0x7

### CHKSUM mode

+---------------+---------------+---------------+-----------------------+
| CHKSUM        | Bit Map       | Bit Symbol    | Description           |
+===============+===============+===============+=======================+
| CMD           | 5 \~ 0        | OP            | OP Code (6\'b001001)  |
|               +---------------+---------------+-----------------------+
|               | 7 \~ 6        | CMD_UNIT      | 2\'b01: 1 Unit.       |
|               +---------------+---------------+-----------------------+
|               | 9 \~ 8        | SIZ_UNIT      | 2\'h0: 1B\            |
|               |               |               | Others: reserve       |
|               +---------------+---------------+-----------------------+
|               | 31 \~ 10      | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| SADDR0        | 31\~0         | SADDR0        | Source Address        |
+---------------+---------------+---------------+-----------------------+
| LEN           | 31\~0         | LEN           | DMA Byte Count        |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| CQ                                                                    |
+---------------+---------------+---------------+-----------------------+
| STA           | 31\~0         | STA           | Status                |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | VAL           | Value of Check Sum    |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
+---------------+---------------+---------------+-----------------------+

### CRC mode

+---------------+---------------+---------------+-----------------------+
| CRC           | Bit Map       | Bit Symbol    | Description           |
+===============+===============+===============+=======================+
| CMD           | 5 \~ 0        | OP            | OP Code (6\'b001010)  |
|               +---------------+---------------+-----------------------+
|               | 7 \~ 6        | CMD_UNIT      | 2\'b01: 1 Unit.       |
|               +---------------+---------------+-----------------------+
|               | 9 \~ 8        | SIZ_UNIT      | 2\'h0: 1B\            |
|               |               |               | Others: reserve       |
|               +---------------+---------------+-----------------------+
|               | 31 \~ 10      | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| SADDR0        | 31\~0         | SADDR0        | Source Address        |
+---------------+---------------+---------------+-----------------------+
| LEN           | 31\~0         | LEN           | DMA Byte Count        |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| PAT0_L        | 31\~0         | PAT0_L        | Pattern 0 Low 4 Byte  |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| CQ                                                                    |
+---------------+---------------+---------------+-----------------------+
| STA           | 31\~0         | STA           | Status                |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | VAL           | Value of CRC32        |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
+---------------+---------------+---------------+-----------------------+

### CONE/CZERO mode

+---------------+---------------+---------------+-----------------------+
| COne/CZero    | Bit Map       | Bit Symbol    | Description           |
+===============+===============+===============+=======================+
| CMD           | 5 \~ 0        | OP            | OP Code (6\'b001011)  |
|               +---------------+---------------+-----------------------+
|               | 7 \~ 6        | CMD_UNIT      | 2\'b01: 1 Unit.       |
|               +---------------+---------------+-----------------------+
|               | 9 \~ 8        | SIZ_UNIT      | 2\'h0: 1B\            |
|               |               |               | Others: reserve       |
|               +---------------+---------------+-----------------------+
|               | 10            | CNT0_EN       | Count Zero Enable     |
|               +---------------+---------------+-----------------------+
|               | 31 \~ 11      | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| SADDR0        | 31\~0         | SADDR0        | Source Address        |
+---------------+---------------+---------------+-----------------------+
| LEN           | 31\~0         | LEN           | DMA Byte Count        |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| CQ                                                                    |
+---------------+---------------+---------------+-----------------------+
| STA           | 31\~0         | STA           | Status                |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | VAL           | Value of COne/CZero   |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
+---------------+---------------+---------------+-----------------------+

### CEQU mode

+---------------+---------------+---------------+------------------------+
| CEQU          | Bit Map       | Bit Symbol    | Description            |
+===============+===============+===============+========================+
| CMD           | 5 \~ 0        | OP            | OP Code (6\'b001100)   |
|               +---------------+---------------+------------------------+
|               | 7 \~ 6        | CMD_UNIT      | 2\'b01: 1 Unit.        |
|               +---------------+---------------+------------------------+
|               | 9 \~ 8        | SIZ_UNIT      | 2\'h0: 1B\             |
|               |               |               | 2\'h1: 2B\             |
|               |               |               | 2\'h2: 4B\             |
|               |               |               | 2\'h3: 8B\             |
|               |               |               | Others: reserve        |
|               +---------------+---------------+------------------------+
|               | 10            | MSK_EN        | Bit Mask Enable        |
|               +---------------+---------------+------------------------+
|               | 31 \~ 11      | Dep           | Depend On FW           |
+---------------+---------------+---------------+------------------------+
| SADDR0        | 31\~0         | SADDR0        | Source Address         |
+---------------+---------------+---------------+------------------------+
| LEN           | 31\~0         | LEN           | DMA Byte Count         |
+---------------+---------------+---------------+------------------------+
| Dep           | 31\~0         | Dep           | Depend On FW           |
+---------------+---------------+---------------+------------------------+
| PAT0_L        | 31\~0         | PAT0_L        | Pattern 0 Low 4 Byte   |
+---------------+---------------+---------------+------------------------+
| PAT0_H        | 31\~0         | PAT0          | Pattern 0 High 4 Byte  |
+---------------+---------------+---------------+------------------------+
| bMSK0_L       | 31\~0         | bMSK0_L       | Bit Mask 0 Low 4 Byte  |
+---------------+---------------+---------------+------------------------+
| bMSK0_H       | 31\~0         | bMSK0_H       | Bit Mask 0 High 4 Byte |
+---------------+---------------+---------------+------------------------+
| CQ                                                                     |
+---------------+---------------+---------------+------------------------+
| STA           | 15\~0         | STA           | Status                 |
|               +---------------+---------------+------------------------+
|               | 31\~16        | Match_CNT     | Match Count            |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
+---------------+---------------+---------------+------------------------+

### CNEQ mode

+---------------+---------------+---------------+------------------------+
| CNEQ          | Bit Map       | Bit Symbol    | Description            |
+===============+===============+===============+========================+
| CMD           | 5 \~ 0        | OP            | OP Code (6\'b001101)   |
|               +---------------+---------------+------------------------+
|               | 7 \~ 6        | CMD_UNIT      | 2\'b01: 1 Unit.        |
|               +---------------+---------------+------------------------+
|               | 9 \~ 8        | SIZ_UNIT      | 2\'h0: 1B\             |
|               |               |               | 2\'h1: 2B\             |
|               |               |               | 2\'h2: 4B\             |
|               |               |               | 2\'h3: 8B\             |
|               |               |               | Others: reserve        |
|               +---------------+---------------+------------------------+
|               | 10            | MSK_EN        | Bit Mask Enable        |
|               +---------------+---------------+------------------------+
|               | 31 \~ 11      | Dep           | Depend On FW           |
+---------------+---------------+---------------+------------------------+
| SADDR0        | 31\~0         | SADDR0        | Source Address         |
+---------------+---------------+---------------+------------------------+
| LEN           | 31\~0         | LEN           | DMA Byte Count         |
+---------------+---------------+---------------+------------------------+
| Dep           | 31\~0         | Dep           | Depend On FW           |
+---------------+---------------+---------------+------------------------+
| PAT0_L        | 31\~0         | PAT0_L        | Pattern 0 Low 4 Byte   |
+---------------+---------------+---------------+------------------------+
| PAT0_H        | 31\~0         | PAT0          | Pattern 0 High 4 Byte  |
+---------------+---------------+---------------+------------------------+
| bMSK0_L       | 31\~0         | bMSK0_L       | Bit Mask 0 Low 4 Byte  |
+---------------+---------------+---------------+------------------------+
| bMSK0_H       | 31\~0         | bMSK0_H       | Bit Mask 0 High 4 Byte |
+---------------+---------------+---------------+------------------------+
| CQ                                                                     |
+---------------+---------------+---------------+------------------------+
| STA           | 15\~0         | STA           | Status                 |
|               +---------------+---------------+------------------------+
|               | 31\~16        | Match_CNT     | Match Count            |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
+---------------+---------------+---------------+------------------------+

### CMORE mode

+---------------+---------------+---------------+------------------------+
| CMORE         | Bit Map       | Bit Symbol    | Description            |
+===============+===============+===============+========================+
| CMD           | 5 \~ 0        | OP            | OP Code (6\'b001110)   |
|               +---------------+---------------+------------------------+
|               | 7 \~ 6        | CMD_UNIT      | 2\'b01: 1 Unit.        |
|               +---------------+---------------+------------------------+
|               | 9 \~ 8        | SIZ_UNIT      | 2\'h0: 1B\             |
|               |               |               | 2\'h1: 2B\             |
|               |               |               | 2\'h2: 4B\             |
|               |               |               | 2\'h3: 8B\             |
|               |               |               | Others: reserve        |
|               +---------------+---------------+------------------------+
|               | 10            | MSK_EN        | Bit Mask Enable        |
|               +---------------+---------------+------------------------+
|               | 31 \~ 11      | Dep           | Depend On FW           |
+---------------+---------------+---------------+------------------------+
| SADDR0        | 31\~0         | SADDR0        | Source Address         |
+---------------+---------------+---------------+------------------------+
| LEN           | 31\~0         | LEN           | DMA Byte Count         |
+---------------+---------------+---------------+------------------------+
| Dep           | 31\~0         | Dep           | Depend On FW           |
+---------------+---------------+---------------+------------------------+
| PAT0_L        | 31\~0         | PAT0_L        | Pattern 0 Low 4 Byte   |
+---------------+---------------+---------------+------------------------+
| PAT0_H        | 31\~0         | PAT0          | Pattern 0 High 4 Byte  |
+---------------+---------------+---------------+------------------------+
| bMSK0_L       | 31\~0         | bMSK0_L       | Bit Mask 0 Low 4 Byte  |
+---------------+---------------+---------------+------------------------+
| bMSK0_H       | 31\~0         | bMSK0_H       | Bit Mask 0 High 4 Byte |
+---------------+---------------+---------------+------------------------+
| CQ                                                                     |
+---------------+---------------+---------------+------------------------+
| STA           | 15\~0         | STA           | Status                 |
|               +---------------+---------------+------------------------+
|               | 31\~16        | Match_CNT     | Match Count            |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
+---------------+---------------+---------------+------------------------+

### CLESS mode

+---------------+---------------+---------------+------------------------+
| CLESS         | Bit Map       | Bit Symbol    | Description            |
+===============+===============+===============+========================+
| CMD           | 5 \~ 0        | OP            | OP Code (6\'b001111)   |
|               +---------------+---------------+------------------------+
|               | 7 \~ 6        | CMD_UNIT      | 2\'b01: 1 Unit.        |
|               +---------------+---------------+------------------------+
|               | 9 \~ 8        | SIZ_UNIT      | 2\'h0: 1B\             |
|               |               |               | 2\'h1: 2B\             |
|               |               |               | 2\'h2: 4B\             |
|               |               |               | 2\'h3: 8B\             |
|               |               |               | Others: reserve        |
|               +---------------+---------------+------------------------+
|               | 10            | MSK_EN        | Bit Mask Enable        |
|               +---------------+---------------+------------------------+
|               | 31 \~ 11      | Dep           | Depend On FW           |
+---------------+---------------+---------------+------------------------+
| SADDR0        | 31\~0         | SADDR0        | Source Address         |
+---------------+---------------+---------------+------------------------+
| LEN           | 31\~0         | LEN           | DMA Byte Count         |
+---------------+---------------+---------------+------------------------+
| Dep           | 31\~0         | Dep           | Depend On FW           |
+---------------+---------------+---------------+------------------------+
| PAT0_L        | 31\~0         | PAT0_L        | Pattern 0 Low 4 Byte   |
+---------------+---------------+---------------+------------------------+
| PAT0_H        | 31\~0         | PAT0          | Pattern 0 High 4 Byte  |
+---------------+---------------+---------------+------------------------+
| bMSK0_L       | 31\~0         | bMSK0_L       | Bit Mask 0 Low 4 Byte  |
+---------------+---------------+---------------+------------------------+
| bMSK0_H       | 31\~0         | bMSK0_H       | Bit Mask 0 High 4 Byte |
+---------------+---------------+---------------+------------------------+
| CQ                                                                     |
+---------------+---------------+---------------+------------------------+
| STA           | 15\~0         | STA           | Status                 |
|               +---------------+---------------+------------------------+
|               | 31\~16        | Match_CNT     | Match Count            |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
|               +---------------+---------------+------------------------+
|               | 31\~0         | RSV.          | Reserved               |
+---------------+---------------+---------------+------------------------+

### CRANG mode

+---------------+---------------+---------------+-----------------------+
| CRANG         | Bit Map       | Bit Symbol    | Description           |
+===============+===============+===============+=======================+
| CMD           | 5 \~ 0        | OP            | OP Code (6\'b010000)  |
|               +---------------+---------------+-----------------------+
|               | 7 \~ 6        | CMD_UNIT      | 2\'b01: 1 Unit.       |
|               +---------------+---------------+-----------------------+
|               | 9 \~ 8        | SIZ_UNIT      | 2\'h0: 1B\            |
|               |               |               | 2\'h1: 2B\            |
|               |               |               | 2\'h2: 4B\            |
|               |               |               | Others: reserve       |
|               +---------------+---------------+-----------------------+
|               | 10            | MSK_EN        | Bit Mask Enable       |
|               +---------------+---------------+-----------------------+
|               | 31 \~ 11      | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| SADDR0        | 31\~0         | SADDR0        | Source Address        |
+---------------+---------------+---------------+-----------------------+
| LEN           | 31\~0         | LEN           | DMA Byte Count        |
+---------------+---------------+---------------+-----------------------+
| bMASK0        | 31\~0         | bMASK0        | Bit Mask              |
+---------------+---------------+---------------+-----------------------+
| PAT0          | 31\~0         | PAT0          | Search Pattern 0      |
+---------------+---------------+---------------+-----------------------+
| PAT1          | 31\~0         | PAT1          | Search Pattern 1      |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| CQ                                                                    |
+---------------+---------------+---------------+-----------------------+
| STA           | 15\~0         | STA           | Status                |
|               +---------------+---------------+-----------------------+
|               | 31\~16        | Match_CNT     | Match Count           |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
+---------------+---------------+---------------+-----------------------+

### SV mode

+---------------+---------------+---------------+-----------------------+
| SV            | Bit Map       | Bit Symbol    | Description           |
+===============+===============+===============+=======================+
| CMD           | 5 \~ 0        | OP            | OP Code (6\'b010001)  |
|               +---------------+---------------+-----------------------+
|               | 7 \~ 6        | CMD_UNIT      | 2\'b01: 1 Unit.       |
|               +---------------+---------------+-----------------------+
|               | 9 \~ 8        | SIZ_UNIT      | 2\'h0: 1B\            |
|               |               |               | 2\'h1: 2B\            |
|               |               |               | 2\'h2: 4B\            |
|               |               |               | 2\'h3: 8B\            |
|               |               |               | Others: reserve       |
|               +---------------+---------------+-----------------------+
|               | 31 \~ 10      | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| SADDR0        | 31\~0         | SADDR0        | Source Address        |
+---------------+---------------+---------------+-----------------------+
| LEN           | 31\~0         | LEN           | DMA Byte Count        |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| PAT0_L        | 31\~0         | PAT0_L        | Pattern 0 Low 4 Byte  |
+---------------+---------------+---------------+-----------------------+
| PAT0_H        | 31\~0         | PAT0_H        | Pattern 0 High 4 Byte |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| Dep           | 31\~0         | Dep           | Depend On FW          |
+---------------+---------------+---------------+-----------------------+
| CQ                                                                    |
+---------------+---------------+---------------+-----------------------+
| STA           | 31\~0         | STA           | Status                |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
|               +---------------+---------------+-----------------------+
|               | 31\~0         | RSV.          | Reserved              |
+---------------+---------------+---------------+-----------------------+

### SHSUM mode

+-----------------+-----------------+-----------------+---------------------------+
| SHSUM           | Bit Map         | Bit Symbol      | Description               |
+=================+=================+=================+===========================+
| CMD             | 5 \~ 0          | OP              | OP Code(6\'b010010).      |
|                 +-----------------+-----------------+---------------------------+
|                 | 7\~6            | CMD_UNIT        | 2 or 3 UNIT               |
|                 +-----------------+-----------------+---------------------------+
|                 | 9 \~ 8          | SIZ_UNIT        | 2\'h2: 4B\                |
|                 |                 |                 | Others: reserve           |
|                 +-----------------+-----------------+---------------------------+
|                 | 10              | MSK_EN          | Bit Mask Enable           |
|                 +-----------------+-----------------+---------------------------+
|                 | 12 \~ 11        | Dep             | Depend On FW              |
|                 +-----------------+-----------------+---------------------------+
|                 | 15 \~ 13        | Target          | One - Eight Target (0\~7) |
|                 +-----------------+-----------------+---------------------------+
|                 | 18 \~ 16        | bSHIFT0         | Shift Bit 0               |
|                 +-----------------+-----------------+---------------------------+
|                 | 21 \~ 19        | Dep             | Depend On FW              |
|                 +-----------------+-----------------+---------------------------+
|                 | 23 \~ 22        | Dep             | Depend On FW              |
|                 +-----------------+-----------------+---------------------------+
|                 | 31 \~ 24        | Dep             | Depend On FW              |
+-----------------+-----------------+-----------------+---------------------------+
| SADDR0          | 31\~0           | SADDR0          | Source Address            |
+-----------------+-----------------+-----------------+---------------------------+
| LEN             | 31\~0           | LEN             | DMA Byte Count            |
+-----------------+-----------------+-----------------+---------------------------+
| bMASK0          | 31\~0           | bMASK0          | Bit Mask 0 Low 4 Byte     |
+-----------------+-----------------+-----------------+---------------------------+
| PAT0_L          | 31\~0           | PAT0            | Search Pattern 0          |
+-----------------+-----------------+-----------------+---------------------------+
| PAT1_L          | 31\~0           | PAT1            | Search Pattern 1          |
+-----------------+-----------------+-----------------+---------------------------+
| PAT2_L          | 31\~0           | PAT0            | Search Pattern 2          |
+-----------------+-----------------+-----------------+---------------------------+
| PAT3_L          | 31\~0           | PAT1            | Search Pattern 3          |
+-----------------+-----------------+-----------------+---------------------------+
| SQ2                                                                             |
+-----------------+-----------------+-----------------+---------------------------+
| PAT4_L          | 31\~0           | PAT4            | Search Pattern 4          |
+-----------------+-----------------+-----------------+---------------------------+
| PAT5_L          | 31\~0           | PAT5            | Search Pattern 5          |
+-----------------+-----------------+-----------------+---------------------------+
| PAT6_L          | 31\~0           | PAT6            | Search Pattern 6          |
+-----------------+-----------------+-----------------+---------------------------+
| PAT7_L          | 31\~0           | PAT7            | Search Pattern 7          |
+-----------------+-----------------+-----------------+---------------------------+
| bMASK1_L        | 31\~0           | bMASK1_L        | Bit Mask 1 Low 4 Byte     |
+-----------------+-----------------+-----------------+---------------------------+
| CQ                                                                              |
+-----------------+-----------------+-----------------+---------------------------+
| STA             | 7\~0            | STA             | Status                    |
|                 +-----------------+-----------------+---------------------------+
|                 | 8               | Match\[0\]      | Match or not              |
|                 +-----------------+-----------------+---------------------------+
|                 | 9               | Match\[1\]      | Match or not              |
|                 +-----------------+-----------------+---------------------------+
|                 | 10              | Match\[2\]      | Match or not              |
|                 +-----------------+-----------------+---------------------------+
|                 | 11              | Match\[3\]      | Match or not              |
|                 +-----------------+-----------------+---------------------------+
|                 | 12              | Match\[4\]      | Match or not              |
|                 +-----------------+-----------------+---------------------------+
|                 | 13              | Match\[5\]      | Match or not              |
|                 +-----------------+-----------------+---------------------------+
|                 | 14              | Match\[6\]      | Match or not              |
|                 +-----------------+-----------------+---------------------------+
|                 | 15              | Match\[7\]      | Match or not (FMR_EN)     |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~16          | RSV.            | Reserved                  |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | RSV.            | Reserved                  |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | INDEX0.         | INDEX 0                   |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | VAL0            | Value 0                   |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | INDEX1.         | INDEX 1                   |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | VAL1            | Value 1                   |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | INDEX2.         | INDEX 2                   |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | VAL2            | Value 2                   |
+-----------------+-----------------+-----------------+---------------------------+
| CQ2                                                                             |
+-----------------+-----------------+-----------------+---------------------------+
| STA             | 31\~0           | INDEX3          | INDEX 3                   |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | VAL3            | Value 3                   |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | INDEX4.         | INDEX 4                   |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | VAL4            | Value 4                   |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | INDEX5.         | INDEX 5                   |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | VAL5            | Value 5                   |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | INDEX6.         | INDEX 6                   |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | VAL6            | Value 6                   |
+-----------------+-----------------+-----------------+---------------------------+
| CQ3                                                                             |
+-----------------+-----------------+-----------------+---------------------------+
| STA             | 31\~0           | INDEX7.         | INDEX 7                   |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | VAL7            | Value 7                   |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | RSV.            | Reserved                  |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | RSV.            | Reserved                  |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | RSV.            | Reserved                  |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | RSV.            | Reserved                  |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | RSV.            | Reserved                  |
|                 +-----------------+-----------------+---------------------------+
|                 | 31\~0           | RSV.            | Reserved                  |
+-----------------+-----------------+-----------------+---------------------------+

**Note**: Please refer to chapter 3.2 and 3.3 to get SQ/CQ format with Unit\>=2.

+----------------------------------------------------------------------------------------------------------------------------------------------------+
| ![D:\\matt_cheng\\Desktop\\00.png](D:\AI_DEV\Output\PS3111_RegSpec_SS_DMAC_assets/media/image1.png){width="6.877777777777778in" height="1.9125in"} |
|                                                                                                                                                    |
| - bit length of mask0 = 32, and bit length of mask1 = 8.                                                                                           |
|                                                                                                                                                    |
| - shift bit = the first not zero LSB bit of mask1. i.e. mask1=8'b10111000, the first not zero bit of mask1 is bit3, which means the shift bit = 3. |
+====================================================================================================================================================+

**Note**: SHSUM (search hit and summation) will search matched pattern, return the matched index, and sum the value in the sum zone of each 4 Byte from start address to matched pattern address.

### RDATA mode

+---------------+---------------+---------------+----------------------------+
| RDATA         | Bit Map       | Bit Symbol    | Description                |
+===============+===============+===============+============================+
| CMD           | 5 \~ 0        | OP            | OP Code (6\'b010011)       |
|               +---------------+---------------+----------------------------+
|               | 7 \~ 6        | CMD_UNIT      | 2\'b01: 1 Unit.            |
|               +---------------+---------------+----------------------------+
|               | 11 \~ 8       | Dep           | Depend On FW               |
|               +---------------+---------------+----------------------------+
|               | 12            | CONT_EN       | Enable Continuous Search   |
|               +---------------+---------------+----------------------------+
|               | 15\~13        | Target        | One -- Seven Target (0\~6) |
|               +---------------+---------------+----------------------------+
|               | 31\~16        | Dep           | Depend On FW               |
+---------------+---------------+---------------+----------------------------+
| SADDR0        | 31\~0         | SADDR0        | Source0 Address            |
+---------------+---------------+---------------+----------------------------+
| SADDR1        | 31\~0         | SADDR1        | Source1 Address            |
+---------------+---------------+---------------+----------------------------+
| SADDR2        | 31\~0         | SADDR2        | Source2 Address            |
+---------------+---------------+---------------+----------------------------+
| SADDR3        | 31\~0         | SADDR3        | Source3 Address            |
+---------------+---------------+---------------+----------------------------+
| SADDR4        | 31\~0         | SADDR4        | Source4 Address            |
+---------------+---------------+---------------+----------------------------+
| SADDR5        | 31\~0         | SADDR5        | Source5 Address            |
+---------------+---------------+---------------+----------------------------+
| SADDR6        | 31\~0         | SADDR6        | Source6 Address            |
+---------------+---------------+---------------+----------------------------+
| CQ                                                                         |
+---------------+---------------+---------------+----------------------------+
| STA           | 31\~0         | STA           | Status                     |
|               +---------------+---------------+----------------------------+
|               | 31\~0         | VAL0          | Read Data Value0           |
|               +---------------+---------------+----------------------------+
|               | 31\~0         | VAL1          | Read Data Value1           |
|               +---------------+---------------+----------------------------+
|               | 31\~0         | VAL2          | Read Data Value2           |
|               +---------------+---------------+----------------------------+
|               | 31\~0         | VAL3.         | Read Data Value3           |
|               +---------------+---------------+----------------------------+
|               | 31\~0         | VAL4          | Read Data Value4           |
|               +---------------+---------------+----------------------------+
|               | 31\~0         | VAL5          | Read Data Value5           |
|               +---------------+---------------+----------------------------+
|               | 31\~0         | VAL6          | Read Data Value6           |
+---------------+---------------+---------------+----------------------------+

**Note**: RDATA (read data) can read up to seven 4 Byte data, and those 4 Byte data can be continuous or discrete.

### NMIN mode

+---------------+---------------+---------------+-------------------------+
| NMIN          | Bit Map       | Bit Symbol    | Description             |
+===============+===============+===============+=========================+
| CMD           | 5 \~ 0        | OP            | OP Code (6\'b010100)    |
|               +---------------+---------------+-------------------------+
|               | 7 \~ 6        | CMD_UNIT      | 2\'b11: 3 Unit.         |
|               +---------------+---------------+-------------------------+
|               | 9 \~ 8        | SIZE_UNIT     | 2\'h3: 8B               |
|               +---------------+---------------+-------------------------+
|               | 10            | MSK_EN        | Bit Mask Enable         |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 11      | Dep           | Depend On FW            |
+---------------+---------------+---------------+-------------------------+
| SADDR0        | 31\~0         | SADDR0        | Source Address          |
+---------------+---------------+---------------+-------------------------+
| LEN           | 31\~0         | LEN           | DMA Byte Count          |
+---------------+---------------+---------------+-------------------------+
| Dep           | 31\~0         | Dep           | Depend On FW            |
+---------------+---------------+---------------+-------------------------+
| Dep           | 31\~0         | Dep           | Depend On FW            |
+---------------+---------------+---------------+-------------------------+
| Dep           | 31\~0         | Dep           | Depend On FW            |
+---------------+---------------+---------------+-------------------------+
| bMSK0_L       | 31\~0         | bMSK0_L       | Bit Mask 0 Low 4 Byte   |
+---------------+---------------+---------------+-------------------------+
| Dep           | 31\~0         | Dep           | Depend On FW            |
+---------------+---------------+---------------+-------------------------+
| CQ                                                                      |
+---------------+---------------+---------------+-------------------------+
| STA           | 31 \~ 0       | STA           | Status                  |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 0       | RSV.          | Reserved                |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 0       | SH_RESULT0    | Search Result 0         |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 0       | SH_VAL0       | Search Val0 Low 4 Byte  |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 0       | SH_RESULT1    | Search Result 1         |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 0       | SH_VAL1       | Search Val 1 Low 4 Byte |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 0       | SH_RESULT2    | Search Result 2         |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 0       | SH_VAL2       | Search Val 2 Low 4 Byte |
+---------------+---------------+---------------+-------------------------+
| CQ2                                                                     |
+---------------+---------------+---------------+-------------------------+
| STA           | 31 \~ 0       | SH_RESULT3    | Search Result 3         |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 0       | SH_VAL3       | Search Val 3 Low 4 Byte |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 0       | SH_RESULT4    | Search Result 4         |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 0       | SH_VAL4       | Search Val 4 Low 4 Byte |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 0       | SH_RESULT5    | Search Result 5         |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 0       | SH_VAL5       | Search Val 5 Low 4 Byte |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 0       | SH_RESULT6    | Search Result 6         |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 0       | SH_VAL6       | Search Val 6 Low 4 Byte |
+---------------+---------------+---------------+-------------------------+
| CQ3                                                                     |
+---------------+---------------+---------------+-------------------------+
| STA           | 31 \~ 0       | SH_RESULT7    | Search Result 7         |
|               +---------------+---------------+-------------------------+
|               | 31 \~ 0       | SH_VAL7       | Search Val 7 Low 4 Byte |
+---------------+---------------+---------------+-------------------------+
|               | 31\~0         | RSV.          | Reserved                |
+---------------+---------------+---------------+-------------------------+
|               | 31\~0         | RSV.          | Reserved                |
+---------------+---------------+---------------+-------------------------+
|               | 31\~0         | RSV.          | Reserved                |
+---------------+---------------+---------------+-------------------------+
|               | 31\~0         | RSV.          | Reserved                |
+---------------+---------------+---------------+-------------------------+
|               | 31\~0         | RSV.          | Reserved                |
+---------------+---------------+---------------+-------------------------+
|               | 31\~0         | RSV.          | Reserved                |
+---------------+---------------+---------------+-------------------------+

**Note**: NMIN will return 8 minimum values for searching each 8 byte data's low 4 byte, and return relevant offset address.(SH_RESULT7 has the minimum value SH_VAL7)

**Note2**: Please refer to chapter 3.3 to get SQ/CQ format with Unit=3.

### BLOOM mode

+---------------+---------------+---------------+--------------------------+
| BF            | Bit Map       | Bit Symbol    | Description              |
+===============+===============+===============+==========================+
| CMD           | 5 \~ 0        | OP            | OP Code (6\'b010101)     |
|               +---------------+---------------+--------------------------+
|               | 7 \~ 6        | CMD_UNIT      | 2\'b01: 1 Unit.          |
|               +---------------+---------------+--------------------------+
|               | 9 \~ 8        | Dep           | Depend On FW             |
|               +---------------+---------------+--------------------------+
|               | 10            | Dep           | Depend On FW             |
|               +---------------+---------------+--------------------------+
|               | 11            | SET_EN        | Set Enable               |
|               +---------------+---------------+--------------------------+
|               | 16\~12        | TIME          | Calculation Time (1\~16) |
|               +---------------+---------------+--------------------------+
|               | 31\~17        | Dep           | Depend On FW             |
+---------------+---------------+---------------+--------------------------+
| SADDR0        | 31\~0         | SADDR0        | Source Address           |
+---------------+---------------+---------------+--------------------------+
| LEN           | 31\~0         | LEN           | Bit Count                |
+---------------+---------------+---------------+--------------------------+
| BF_DATA       | 31\~0         | DATA          | Bloom Filter Data        |
+---------------+---------------+---------------+--------------------------+
| XOR           | 31\~0         | XOR           | XOR Value                |
+---------------+---------------+---------------+--------------------------+
| MULT          | 31\~0         | MULT          | Multiply Value           |
+---------------+---------------+---------------+--------------------------+
| Dep           | 31\~0         | Dep           | Depend On FW             |
+---------------+---------------+---------------+--------------------------+
| Dep           | 31\~0         | Dep           | Depend On FW             |
+---------------+---------------+---------------+--------------------------+
| CQ                                                                       |
+---------------+---------------+---------------+--------------------------+
| STA           | 0             | HIT           | 0:Hit , 1:Not Hit        |
|               +---------------+---------------+--------------------------+
|               | 31\~1         | RSV.          | Reserved                 |
|               +---------------+---------------+--------------------------+
|               | 31\~0         | RSV.          | Reserved                 |
|               +---------------+---------------+--------------------------+
|               | 31\~0         | RSV.          | Reserved                 |
|               +---------------+---------------+--------------------------+
|               | 31\~0         | RSV.          | Reserved                 |
|               +---------------+---------------+--------------------------+
|               | 31\~0         | RSV.          | Reserved                 |
|               +---------------+---------------+--------------------------+
|               | 31\~0         | RSV.          | Reserved                 |
|               +---------------+---------------+--------------------------+
|               | 31\~0         | RSV.          | Reserved                 |
|               +---------------+---------------+--------------------------+
|               | 31\~0         | RSV.          | Reserved                 |
+---------------+---------------+---------------+--------------------------+

**Note**: BLOOM (Bloom filter) mode will check whether BF_DATA

hits the existed data or not, and if the SET_EN is set, SS will set up the relevant bit of BF_DATA after checking BF_DATA, no matter BF_DATA hits or not.

**Note2**: One cannot confirm that SS is done in BLOOM mode when SS return CQ.

![](D:\AI_DEV\Output\PS3111_RegSpec_SS_DMAC_assets/media/image2.png){width="4.490972222222222in" height="3.154166666666667in"}**Example1: (Not Hit, Definitely not there)**

existed data=\[1,4,12,15\], BF_DATA=7

**Example2: (Hit, Probably there)**

existed data=\[1,4,12,15\], BF_DATA=![](D:\AI_DEV\Output\PS3111_RegSpec_SS_DMAC_assets/media/image3.png){width="4.486805555555556in" height="3.15625in"}36

## SS Control Flow and Note

+------------+-----------------------------------------------------------------------------------------------------------+--------------------------------------------+
| **Step 1** | **Submission Queue setting and Completion Queue setting**                                                 |                                            |
|            |                                                                                                           |                                            |
|            | **(Set QInfo)**                                                                                           |                                            |
|            +-----------------------------------------------------------------------------------------------------------+--------------------------------------------+
|            | // SQ setting\                                                                                            | // Submission Queue Base Address\          |
|            | dDRAM0_0(0x40) = 0x5C0CA000;\                                                                             | // Buffer Length (example: 1KB = 32B\*32)\ |
|            | wDRAM0_0(0x54) = 0x20;\                                                                                   | // Data Size: 0x05 (32 Byte)\              |
|            | bDRAM0_0(0x56) = 0x05;\                                                                                   | // Control: Valid Queue                    |
|            | bDRAM0_0(0x57) = 0x01;                                                                                    |                                            |
|            +-----------------------------------------------------------------------------------------------------------+--------------------------------------------+
|            | // CQ setting\                                                                                            | // Completion Queue Base Address\          |
|            | dDRAM0_0(0x60) = 0x5C0CB000;\                                                                             | // Buffer Length (example: 1KB = 32B\*32)\ |
|            | wDRAM0_0(0x74) = 0x20;\                                                                                   | // Data Size: 0x05 (32 Byte)\              |
|            | bDRAM0_0(0x76) = 0x05;\                                                                                   | // Control: Valid Queue                    |
|            | bDRAM0_0(0x77) = 0x01;                                                                                    |                                            |
+:===========+:==========================================================================================================+:===========================================+
| **Step 2** | **Putting the command setting to SQ.It can continually put the command setting to SQ until the SQ full.** |                                            |
|            |                                                                                                           |                                            |
|            | **(Set QBody)**                                                                                           |                                            |
|            +-----------------------------------------------------------------------------------------------------------+--------------------------------------------+
|            | while(!(bDRAM0_0(0x6e)\<0x02));                                                                           | // writable Count for SQ                   |
|            |                                                                                                           |                                            |
|            | // put command setting to SQ0\                                                                            | // Command (Set Value mode)\               |
|            | i = 0x00;\                                                                                                | // Source Address (DCCM)\                  |
|            | dDRAM0_1(0x00+i) = 0x00000051;\                                                                           | // Byte Length\                            |
|            | dDRAM0_1(0x04+i) = 0x5C0D5497;\                                                                           | // Reserved\                               |
|            | dDRAM0_1(0x08+i) = 0x00000D71;\                                                                           | // Pattern_L\                              |
|            | dDRAM0_1(0x0C+i) = 0x00000000;\                                                                           | // Pattern_H\                              |
|            | dDRAM0_1(0x10+i) = 0x0000005D;\                                                                           | // bMSK0_L\                                |
|            | dDRAM0_1(0x14+i) = 0x00000000;\                                                                           | // bMSK0_H\                                |
|            | dDRAM0_1(0x18+i) = 0x00000000;\                                                                           | \                                          |
|            | dDRAM0_1(0x1C+i) = 0x00000000;\                                                                           | \                                          |
|            | // put command setting to SQ1\                                                                            | // Command (Set Value mode)\               |
|            | i = 0x20;\                                                                                                | // Source Address (DDR_Z2)\                |
|            | dDRAM0_1(0x00+i) = 0x00000051;\                                                                           | // Byte Length\                            |
|            | dDRAM0_1(0x04+i) = 0x63021D80;\                                                                           | // Reserved\                               |
|            | dDRAM0_1(0x08+i) = 0x00000CE0;\                                                                           | // Pattern_L\                              |
|            | dDRAM0_1(0x0C+i) = 0x00000000;\                                                                           | // Pattern_H\                              |
|            | dDRAM0_1(0x10+i) = 0x000000ED;\                                                                           | // bMSK0_L\                                |
|            | dDRAM0_1(0x14+i) = 0x00000000;\                                                                           | // bMSK0_H                                 |
|            | dDRAM0_1(0x18+i) = 0x00000000;\                                                                           |                                            |
|            | dDRAM0_1(0x1C+i) = 0x00000000;                                                                            |                                            |
+------------+-----------------------------------------------------------------------------------------------------------+--------------------------------------------+
| **Step 3** | **Set SQ Write Pointer Increase Unit**                                                                    |                                            |
|            +-----------------------------------------------------------------------------------------------------------+--------------------------------------------+
|            | wDRAM0_0(0x4C) = 0x02;                                                                                    | // send 2 SQ to SS (trigger)               |
+------------+-----------------------------------------------------------------------------------------------------------+--------------------------------------------+
| **Step 4** | **SS will put a status information to the CQ, FW can use RD_CNT to check whether SS is done or not.**     |                                            |
|            +-----------------------------------------------------------------------------------------------------------+--------------------------------------------+
|            | while(!(bDRAM0_0(0x72)\<0x02));                                                                           | // Readable Count for CQ                   |
+------------+-----------------------------------------------------------------------------------------------------------+--------------------------------------------+
| **Step 5** | **Getting status information from the CQ, and analyzing result of the command operation.**                |                                            |
+------------+-----------------------------------------------------------------------------------------------------------+--------------------------------------------+
| **Note**   | Start address of each SQ/CQ need align with 32Byte                                                                                                     |
+------------+--------------------------------------------------------------------------------------------------------------------------------------------------------+

# DMAC Memory Matrix

+----------------+----------+-----------------+------------------+-----------------+--------+---------+----------+
| CP Path Matrix | To       | DRAM (Z1, Code) | DRAM (Z2, Table) | DRAM (Z3, Data) | DBUF   | CPU.MEM | FLH.IRAM |
+----------------+----------+                 |                  |                 |        |         |          |
| From           | 　       |                 |                  |                 |        |         |          |
+================+==========+:===============:+:================:+:===============:+:======:+:=======:+:========:+
| DRAM (Z1, Code)           | B               | B                | C1              | C1     | B       | B        |
+---------------------------+-----------------+------------------+-----------------+--------+---------+----------+
| DRAM (Z2, Table)          | B               | B                | C1              | C1     | B       | B        |
+---------------------------+-----------------+------------------+-----------------+--------+---------+----------+
| DRAM (Z3, Data)           | E1              | E1               | F1              | F1     | E1      | E1       |
+---------------------------+-----------------+------------------+-----------------+--------+---------+----------+
| DBUF                      | E1              | E1               | F1              | F1     | E1      | E1       |
+---------------------------+-----------------+------------------+-----------------+--------+---------+----------+
| CPU.MEM                   | B               | B                | C1              | C1     | B       | B        |
+---------------------------+-----------------+------------------+-----------------+--------+---------+----------+
| FLH.IRAM                  | B               | B                | C1              | C1     | B       | B        |
+---------------------------+-----------------+------------------+-----------------+--------+---------+----------+

+-----------+----+-----------------+------------------+-----------------+--------+---------+----------+
| SV Matrix | To | DRAM (Z1, Code) | DRAM (Z2, Table) | DRAM (Z3, Data) | DBUF   | CPU.MEM | FLH.IRAM |
+-----------+----+                 |                  |                 |        |         |          |
| Function  | 　 |                 |                  |                 |        |         |          |
+===========+====+:===============:+:================:+:===============:+:======:+:=======:+:========:+
| SV        | 　 | A               | B                | F2              | F2     | A       | A        |
+-----------+----+-----------------+------------------+-----------------+--------+---------+----------+

+-----------------+------------+-----------------+
| E3D Path Matrix | To         | NA              |
+-----------------+------------+                 |
| From            | 　         |                 |
+=================+============+:===============:+
| DRAM (Z1, Code) or           |                 |
|                              |                 |
| DRAM (Z2, Table)             |                 |
+------------------------------+-----------------+
| DRAM (Z3, Data) or           | F2              |
|                              |                 |
| DBUF                         |                 |
+------------------------------+-----------------+
| CPU.MEM                      |                 |
+------------------------------+-----------------+
| FLH.IRAM                     |                 |
+------------------------------+-----------------+

+------------------+------------+-----------------+
| PZIP Path Matrix | To         | NA              |
+------------------+------------+                 |
| From             | 　         |                 |
+==================+============+:===============:+
| DRAM (Z1, Code) or            | F2              |
|                               |                 |
| DRAM (Z2, Table)              |                 |
+-------------------------------+-----------------+
| DRAM (Z3, Data) or            | F2              |
|                               |                 |
| DBUF                          |                 |
+-------------------------------+-----------------+
| CPU.MEM                       |                 |
+-------------------------------+-----------------+
| FLH.IRAM                      |                 |
+-------------------------------+-----------------+

(Note: address need 512Byte alignment even if in Z1/Z2)

+-----------------+-----------+-----------------+------------------+-----------------+--------+---------+----------+
| XOR Path Matrix | To        | DRAM (Z1, Code) | DRAM (Z2, Table) | DRAM (Z3, Data) | DBUF   | CPU.MEM | FLH.IRAM |
+-----------------+-----------+                 |                  |                 |        |         |          |
| From            | 　        |                 |                  |                 |        |         |          |
+=================+===========+:===============:+:================:+:===============:+:======:+:=======:+:========:+
| DRAM (Z1, Code) or          | F4              | F4               | F2              | F2     | 　      | 　       |
|                             |                 |                  |                 |        |         |          |
| DRAM (Z2, Table)            |                 |                  |                 |        |         |          |
|                             |                 |                  |                 |        +---------+----------+
|                             |                 |                  |                 |        | 　      | 　       |
+-----------------------------+-----------------+------------------+-----------------+--------+---------+----------+
| DRAM (Z3, Data) or          | F3              | F3               | F2              | F2     | 　      | 　       |
|                             |                 |                  |                 |        |         |          |
| DBUF                        |                 |                  |                 |        |         |          |
|                             |                 |                  |                 |        +---------+----------+
|                             |                 |                  |                 |        | 　      | 　       |
+-----------------------------+-----------------+------------------+-----------------+--------+---------+----------+
| CPU.MEM                     | 　              | 　               | 　              | 　     | 　      | 　       |
+-----------------------------+-----------------+------------------+-----------------+--------+---------+----------+
| FLH.IRAM                    | 　              | 　               | 　              | 　     | 　      | 　       |
+-----------------------------+-----------------+------------------+-----------------+--------+---------+----------+

(**Note:** source1 and source2 in XOR mode must come from the same memory area, i.e. source1 and source2 both come from DRAM_Z1/DRAM_Z2 or both come from DRAM_Z3/DBUF)

+-----------------+------------+-----------------+
| CMP Path Matrix | To         | NA              |
+-----------------+------------+                 |
| From            | 　         |                 |
+=================+============+:===============:+
| DRAM (Z1, Code) or           | B               |
|                              |                 |
| DRAM (Z2, Table)             |                 |
+------------------------------+-----------------+
| DRAM (Z3, Data) or           | F2              |
|                              |                 |
| DBUF                         |                 |
+------------------------------+-----------------+
| CPU.MEM                      |                 |
+------------------------------+-----------------+
| FLH.IRAM                     |                 |
+------------------------------+-----------------+

(**Note:** source1 and source2 in CMP mode must from the same memory area, i.e. source1 and source2 both come from DRAM_Z1/DRAM_Z2 or both come from DRAM_Z3/DBUF)

+-----------------+------------+-----------------+
| CRC Path Matrix | To         | NA              |
+-----------------+------------+                 |
| From            | 　         |                 |
+=================+============+:===============:+
| DRAM (Z1, Code) or           | B               |
|                              |                 |
| DRAM (Z2, Table)             |                 |
+------------------------------+-----------------+
| DRAM (Z3, Data) or           | F2              |
|                              |                 |
| DBUF                         |                 |
+------------------------------+-----------------+
| CPU.MEM                      | B               |
+------------------------------+-----------------+
| FLH.IRAM                     |                 |
+------------------------------+-----------------+

  --------------------------------------------------------------------------
   FW limitation  Address Alignment   Byte Count          E3D
  --------------- ------------------- ------------------- ------------------
         A        1B \*1              1B\*N               \-

         B        32B                 32B\*N              \-

        C1        S:32B, T:512B       512B\*N             Handle

        E1        S:512B, T:32B       512B\*N             Discard

        F1        512B                512B\*N             Copy

        F2        512B                512B\*N             Handle

        F3        S:512B, T:512B      512B\*N             Discard

        F4        S:512B, T:512B      512B\*N             \-
  --------------------------------------------------------------------------

# SS Memory Matrix

+--------------+--------+-------+-------+-------+-------+-------+-------+
| **Group**    |        |       |       |       |       |       |       |
+==============+========+=======+=======+=======+=======+=======+=======+
| SV           | SV     |       |       |       |       |       |       |
+--------------+--------+-------+-------+-------+-------+-------+-------+
| SS_RW        | MSV    | INC   |       |       |       |       |       |
+--------------+--------+-------+-------+-------+-------+-------+-------+
| SCM          | SEQUx  | SNEQ  | SMORE | SLESS | SRANG | CEQU  | CNEQ  |
|              +--------+-------+-------+-------+-------+-------+-------+
|              | CMORE  | CLESS | CRANG | SMAX  | SMIN  |       |       |
+--------------+--------+-------+-------+-------+-------+-------+-------+
| COPY         | COPY   |       |       |       |       |       |       |
+--------------+--------+-------+-------+-------+-------+-------+-------+
| CAL1         | CONE   | CZERO |       |       |       |       |       |
+--------------+--------+-------+-------+-------+-------+-------+-------+
| CAL2         | CHKSUM | CRC   |       |       |       |       |       |
+--------------+--------+-------+-------+-------+-------+-------+-------+
| RDATA        | RDATA  |       |       |       |       |       |       |
+--------------+--------+-------+-------+-------+-------+-------+-------+
| NMIN         | NMIN   |       |       |       |       |       |       |
+--------------+--------+-------+-------+-------+-------+-------+-------+
| SHSUM        | SHSUM  |       |       |       |       |       |       |
+--------------+--------+-------+-------+-------+-------+-------+-------+
| BLOOM        | BLOOM  |       |       |       |       |       |       |
+--------------+--------+-------+-------+-------+-------+-------+-------+

+----------------+-------+------------+-------------+---------+
| SS Path Matrix | From  | DRAM\      | DRAM\       | CPU.MEM |
|                |       | (Z1, Code) | (Z2, Table) |         |
+----------------+-------+            |             +---------+
| Function       | 　    |            |             | 　      |
+================+=======+:==========:+:===========:+:=======:+
| SV             | 　    | A          | B           | A       |
+----------------+-------+------------+-------------+---------+
| SS_RW          | 　    | A          | A           | A       |
+----------------+-------+------------+-------------+---------+
| SCM            | 　    | A          | A           | A       |
+----------------+-------+------------+-------------+---------+
| CAL1           |       | A          | A           | A       |
+----------------+-------+------------+-------------+---------+
| CAL2           | 　    | D          | D           | D       |
+----------------+-------+------------+-------------+---------+
| RDATA          |       | C          | C           | C       |
+----------------+-------+------------+-------------+---------+
| NMIN           |       | E          | E           | E       |
+----------------+-------+------------+-------------+---------+
| SHSUM          |       | F          | F           | F       |
+----------------+-------+------------+-------------+---------+
| BLOOM          |       | G          | G           | G       |
+----------------+-------+------------+-------------+---------+

+------------------+-------+------------+-------------+---------+
| COPY Path Matrix | TO    | DRAM\      | DRAM\       | CPU.MEM |
|                  |       | (Z1, Code) | (Z2, Table) |         |
+------------------+-------+            |             +---------+
| FROM             | 　    |            |             | 　      |
+==================+=======+:==========:+:===========:+:=======:+
| DRAM (Z1, Code)  | 　    | B          | B           | B       |
+------------------+-------+------------+-------------+---------+
| DRAM (Z2, Table) | 　    | B          | B           | B       |
+------------------+-------+------------+-------------+---------+
| CPU.MEM          | 　    | B          | B           | B       |
+------------------+-------+------------+-------------+---------+

(**Note**:When in COPY mode, if you don't want to follow the rule that addr/len need align 32 byte, the source and destination address LSB 5 bits should be equal.)

  -------------------------------------------------------------------------------------
   FW limitation  Address Alignment   Byte Count    E3D     Note
  --------------- ------------------- ------------- ------- ---------------------------
         A        1B \*1              1B\*N         \-      AXI packet length is 32B.

         B        32B                 32B\*N        \-      AXI packet length is 32B.

         C        4B\*1               \-                    AXI packet length is 32B.

         D        32B                 8B\*N         \-      AXI packet length is 32B.

         E        8B\*1               8B\*N                 AXI packet length is 32B.

         F        32B                 4B\*N                 AXI packet length is 32B.

         G        32B                 1bit \<\< N           AXI packet length is 32B.
  -------------------------------------------------------------------------------------

# PS3111 Memory Map

+-----------------------------------------------+--------------------+-------------------+
| Memory Components                             | CPU Address        | System Address    |
+:=====================:+:=====================:+:==================:+:=================:+
| ICCM0(128KB)                                  | 0x5C0E_0000 to\    | 0x5C0E_0000 to\   |
|                                               | 0x5C0F_FFFF        | 0x5C0F_FFFF       |
+-----------------------------------------------+--------------------+-------------------+
| IROM(32KB)                                    | 0x5C0D_8000 to\    | 0x5C0D_8000 to\   |
|                                               | 0x5C0D_FFFF        | 0x5C0D_FFFF       |
+-----------------------------------------------+--------------------+-------------------+
| DCCM0 Bank0- Q info(512B)- PD1                | 0x5C0C_0000 to\    | 0x5C0C_0000 to\   |
|                                               | 0x5C0C_01FF        | 0x5C0C_01FF       |
+-----------------------------------------------+--------------------+-------------------+
| DCCM0 Bank1- Q body(32KB)- PD1                | 0x5C0C_8000 to\    | 0x5C0C_8000 to\   |
|                                               | 0x5C0C_FFFF        | 0x5C0C_FFFF       |
+-----------------------------------------------+--------------------+-------------------+
| DCCM1 Bank0 - Local work(16KB) - PD1          | 0x5C0D_0000 to\    | 0x5C0D_0000 to\   |
|                                               | 0x5C0D_3FFF        | 0x5C0D_3FFF       |
+-----------------------------------------------+--------------------+-------------------+
| DCCM1 Bank1 - Local work(16KB) - always-on    | 0x5C0D_4000 to\    | 0x5C0D_4000 to\   |
|                                               | 0x5C0D_7FFF        | 0x5C0D_7FFF       |
+-----------------------------------------------+--------------------+-------------------+
| XLMI(64KB)                                    | 0x5C00_0000 to\    | N/A               |
|                                               | 0x5C00_FFFF        |                   |
+-----------------------------------------------+--------------------+-------------------+
| AXI Register                                  | 0x0000_0000 to\    | 0x0000_0000 to\   |
|                                               | 0x03FF_FFFF        | 0x03FF_FFFF       |
+-----------------------------------------------+--------------------+-------------------+
| BVCI Register                                 | 0x0400_0000 to\    | 0x0400_0000 to\   |
|                                               | 0x04FF_FFFF        | 0x04FF_FFFF       |
+-----------------------+-----------------------+--------------------+-------------------+
| Shadow DBUF\          | DATA (256KB)          | 0x0C00_0000 to\    | 0x0C00_0000 to\   |
| (256KB + CRC16)       |                       | 0x0C03_FFFF        | 0x0C03_FFFF       |
|                       +-----------------------+--------------------+-------------------+
|                       | E3D+CRC16             | 0x0C10_0000\       | 0x0C10_0000\      |
|                       |                       | 0x0C10_07FF        | 0x0C10_07FF       |
+-----------------------+-----------------------+--------------------+-------------------+
| DBUF\                 | DATA (256KB)          | 0x2C00_0000 to\    | 0x2C00_0000 to\   |
| (256KB + CRC16)       |                       | 0x2C03_FFFF        | 0x2C03_FFFF       |
|                       +-----------------------+--------------------+-------------------+
|                       | E3D+ CRC16            | 0x2C10_0000\       | 0x2C10_0000\      |
|                       |                       | 0x2C10_07FF        | 0x2C10_07FF       |
+-----------------------+-----------------------+--------------------+-------------------+
| FLH IRAM (64KB)                               | 0x5000_0000 to\    | 0x5000_0000 to\   |
|                                               | 0x5000_FFFF        | 0x5000_FFFF       |
+-----------------------------------------------+--------------------+-------------------+
| DRAM(2GB)                                     | 0x6000_0000 to\    | 0x6000_0000 to\   |
|                                               | 0xDFFF_FFFF        | 0xDFFF_FFFF       |
+-----------------------------------------------+--------------------+-------------------+
