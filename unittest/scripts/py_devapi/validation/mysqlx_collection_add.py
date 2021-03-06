#@ CollectionAdd: valid operations after add with no documents
|All expected functions are available|
|No additional functions are available|

#@ CollectionAdd: valid operations after add
|All expected functions are available|
|No additional functions are available|

#@ CollectionAdd: valid operations after execute
|All expected functions are available|
|No additional functions are available|

#@# CollectionAdd: Error conditions on add
||Invalid number of arguments in CollectionAdd.add, expected 1 but got 0
||CollectionAdd.add: Argument is expected to be either a document or a list of documents
||CollectionAdd.add: Element #1 is expected to be a JSON expression
||CollectionAdd.add: Invalid data type for _id field, should be a string

#@ Collection.add execution
|Affected Rows Single: 1|
|last_document_id Single:|
|get_last_document_id Single:|
|#last_document_ids Single: 1|
|#get_last_document_ids Single: 1|

|Affected Rows Single Known ID: 1|
|last_document_id Single Known ID: sample_document|
|get_last_document_id Single Known ID: sample_document|
|#last_document_ids Single Known ID: 1|
|#get_last_document_ids Single Known ID: 1|
|#last_document_ids Single Known ID: sample_document|
|#get_last_document_ids Single Known ID: sample_document|


|Affected Rows Multi: 2|
|last_document_id Multi: LogicError: Result.get_last_document_id: document id is not available.|
|get_last_document_id Multi: LogicError: Result.get_last_document_id: document id is not available.|
|#last_document_ids Multi: 2|
|#get_last_document_ids Multi: 2|


|Affected Rows Multi Known IDs: 2|
|last_document_id Multi Known IDs: LogicError: Result.get_last_document_id: document id is not available.|
|get_last_document_id Multi Known IDs: LogicError: Result.get_last_document_id: document id is not available.|

|#last_document_ids Multi Known IDs: 2|
|#get_last_document_ids Multi Known IDs: 2|
|First last_document_ids Multi Known IDs: known_00|
|First get_last_document_ids Multi Known IDs: known_00|
|Second last_document_ids Multi Known IDs: known_01|
|Second get_last_document_ids Multi Known IDs: known_01|

|Affected Rows Empty List: -1|
|last_document_id Empty List: LogicError: Result.get_last_document_id: document id is not available.|
|get_last_document_id Empty List: LogicError: Result.get_last_document_id: document id is not available.|

|#last_document_ids Empty List: 0|
|#get_last_document_ids Empty List: 0|

|Affected Rows Chained: 2|
|Affected Rows Single Expression: 1|
|Affected Rows Mixed List: 2|

