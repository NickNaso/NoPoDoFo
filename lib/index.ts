import { Document as doc } from './document'
import { Dictionary as dict } from './dictionary'
import { Obj as obj } from './object'
import { Page as page } from './page'
import {
    Annotation as annot,
    NPDFAction as action,
    NPDFAnnotation as annotenum,
    NPDFAnnotationFlag as annotflags,
    NPDFAnnotationType as annottype
} from './annotation'
import {
    CheckBox as checkbox,
    Field as field,
    TextField as textfield,
    ListField as listfield
} from './field'
import { Image as image } from './image'
import { Painter as painter } from './painter'
import { Rect as rect } from './rect'
import { Signer as sign } from './signer'
import {Stream as stream} from './stream'
import {Form as form} from './form'
import {ContentsTokenizer as ct} from './parser'

export const Document = doc,
    Page = page,
    Dictionary = dict,
    Obj = obj,
    Annotation = annot,
    NPdfAction = action,
    NPdfAnnotation = annotenum,
    NPdfAnnotationFlag = annotflags,
    NPdfAnnotationType = annottype,
    Field = field,
    Form = form,
    CheckBox = checkbox,
    TextField = textfield,
    ListField = listfield,
    Image = image,
    Painter = painter,
    Rect = rect,
    Signature = sign,
    Stream = stream,
    ContentsTokenizer = ct,
    CONVERSION = 0.0028346456693

